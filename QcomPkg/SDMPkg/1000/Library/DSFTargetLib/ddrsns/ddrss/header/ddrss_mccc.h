/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#ifndef __DDRSS_MCCC_H__
#define __DDRSS_MCCC_H__

#include "HAL_SNS_DDR.h"
#include "mccc_seq_hwiobase.h"
#include "mccc_seq_hwioreg.h"
#include "ddrss.h"

 

/**
 * @brief MCCC one-time settings
 * 
 * @param[in] BOOT_DDR_STRUCT *ddr: Pointer to ddr conifiguration struct
 * @param[in] DDR_CHANNEL channel:  Channel selection
 * 
 * @retval None
 */  
void MCCC_Config(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel);

#endif // #ifdef _MCCC_H_
