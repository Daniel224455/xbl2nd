/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
==============================================================================*/
#include "ddrss_common.h"
#include "mc_common.h"
#include "ddr_shrm_periodic_training.h"

// Saves and restores register values to and from memory using a compressed address list
void DDRSS_Register_Save_Restore_Compressed
(
    SAVE_RESTORE save_restore,  // flag indicating if data should be saved or restored
    REG_BLOCK_COUNTS* cnts_ptr, // pointer to base and offset counts for register block
    uint32* mem_ptr,            // pointer to memory for values
    uint32 size                 // size of memory reserved for values
)
{
    uint16* base_ptr = NULL;
    uint16* offset_ptr = NULL;
    uint32* reg_ptr = NULL;
    size_t base_addr = 0;
    uint16 reg_cnt = 0;
    uint8 i = 0;
    uint8 j = 0;
    
    // Loop while base count is greater than 0
    while(cnts_ptr->bases > 0)
    {   
        // Get pointer to array of base addresses for instances of register block
        base_ptr = (uint16*)(cnts_ptr + 1);
        
        // Get pointer to array of register offsets from base address
        offset_ptr = base_ptr + cnts_ptr->bases;
        
        // Loop through base addresses
        for(i = 0; i < cnts_ptr->bases; i++)
        {
            // Get base address
            base_addr = base_ptr[i] << 12;
            
            // Loop through register offsets
            for(j = 0; j < cnts_ptr->offsets; j++)
            {
                // Abort if reserved memory will be exceeded
                if(((reg_cnt + 1) * sizeof(uint32)) > size)
                {
                    return;
                }
                
                // Get register pointer
                reg_ptr = (uint32*)(base_addr + (offset_ptr[j] << 2));
                
                if(save_restore == SAVE)
                {
                    // Save register
                    mem_ptr[reg_cnt] = *reg_ptr;
                }
                else
                {
                    // Restore register
                    *reg_ptr = mem_ptr[reg_cnt];
                }
                
                // Increment register count
                reg_cnt += 1;
            }
        }
        
        // Go to next register block
        cnts_ptr = (REG_BLOCK_COUNTS*)(offset_ptr + cnts_ptr->offsets);
    }
    
    return;
}
