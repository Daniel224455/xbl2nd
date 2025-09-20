/*==============================================================================
QUALCOMM Proprietary Design Data                                                
Copyright (c) 2016-2018 Qualcomm Technologies Incorporated.                     
All rights reserved.                                                            
==============================================================================*/
 
#ifndef _SHRM_LOAD_RAM_H_
#define _SHRM_LOAD_RAM_H_
 
#define RODATA_SECTION_OFFSET_FULL   0x2100
#define RODATA_SECTION_LENGTH_FULL   0x240
 
#define DATA_SECTION_OFFSET_FULL   0x2340
#define DATA_SECTION_LENGTH_FULL   0x368
 
#define BSS_SECTION_OFFSET_FULL   0x26B0
#define BSS_SECTION_LENGTH_FULL   0x380
 
#define RESETVECTOR_TEXT_SECTION_OFFSET_FULL   0x0
#define RESETVECTOR_TEXT_SECTION_LENGTH_FULL   0xBC
 
#define LEVEL2INTERRUPTV_SECTION_OFFSET_FULL   0x2E4
#define LEVEL2INTERRUPTV_SECTION_LENGTH_FULL   0x8
 
#define LEVEL3INTERRUPTV_SECTION_OFFSET_FULL   0x304
#define LEVEL3INTERRUPTV_SECTION_LENGTH_FULL   0x8
 
#define LEVEL4INTERRUPTV_SECTION_OFFSET_FULL   0x324
#define LEVEL4INTERRUPTV_SECTION_LENGTH_FULL   0x8
 
#define LEVEL5INTERRUPTV_SECTION_OFFSET_FULL   0x344
#define LEVEL5INTERRUPTV_SECTION_LENGTH_FULL   0x8
 
#define DEBUGEXCEPTIONVE_SECTION_OFFSET_FULL   0x368
#define DEBUGEXCEPTIONVE_SECTION_LENGTH_FULL   0xC
 
#define KERNELEXCEPTIONV_SECTION_OFFSET_FULL   0x3A4
#define KERNELEXCEPTIONV_SECTION_LENGTH_FULL   0x8
 
#define USEREXCEPTIONVEC1_SECTION_OFFSET_FULL   0x3C0
#define USEREXCEPTIONVEC1_SECTION_LENGTH_FULL   0x4
 
#define USEREXCEPTIONVEC2_SECTION_OFFSET_FULL   0x3C4
#define USEREXCEPTIONVEC2_SECTION_LENGTH_FULL   0x18
 
#define DOUBLEEXCEPTIONV_SECTION_OFFSET_FULL   0x3E4
#define DOUBLEEXCEPTIONV_SECTION_LENGTH_FULL   0x8
 
#define MAIN_LITERAL_SECTION_OFFSET_FULL   0x400
#define MAIN_LITERAL_SECTION_LENGTH_FULL   0x0
 
#define MAIN_TEXT_SECTION_OFFSET_FULL   0x404
#define MAIN_TEXT_SECTION_LENGTH_FULL   0x10
 
#define TEXT_SECTION_OFFSET_FULL   0x424
#define TEXT_SECTION_LENGTH_FULL   0x5CF8
 
#define SHRM_DRAM0_TOTAL_SIZE_FULL   2344
#define SHRM_IRAM0_TOTAL_SIZE_FULL   24092
#define SHRM_TOTAL_SIZE_FULL         26436
 
/*==============================================================================
                            FUNCTION DECLARATION                                
==============================================================================*/
 
/* =============================================================================
**   SHRM Load Functions                                                   
 ===============================================================================
*/
/**
 * @brief Load IRAM and DRAM of SHRM
 *
 * @param[in] None
 * @retval None
 */ 
void SHRM_Full_Load_SRAM (void);
 
void SHRM_Clear_SHRM_STRUCT_Periodic_training_DRAM_Region (void);
 
#endif
