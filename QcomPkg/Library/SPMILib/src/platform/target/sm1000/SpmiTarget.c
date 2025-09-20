/**
 * @file:  SpmiTarget.c
 * 
 * Copyright (c) 2013-2019 by Qualcomm Technologies Incorporated. All Rights Reserved.
 * 
 * $DateTime: 2021/04/29 02:01:56 $
 * $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Library/SPMILib/src/platform/target/sm1000/SpmiTarget.c#1 $
 * $Change: 30389851 $ 
 * $Author: pwbldsvc $
 *                              Edit History
 * Date     Description
 * -------  ------------------------------------------------------------------- 
 * * 06/24/19  Initial Version
 */
#include "SpmiHal.h"
#include "ChipInfo.h"
#include "SpmiTarget.h"



//******************************************************************************
// Macros / Definitions / Constants
//******************************************************************************

#define HWIO_TLMM_PMIC_HDRV_PULL_CTL_ADDR      0x031B3000
#define HWIO_TLMM_PMIC_HDRV_PULL_CTL_DATA      0x00FF0800

/**
 * Static Function Declarations and Definitions
 */


//******************************************************************************
// Global Data
//******************************************************************************




//******************************************************************************
// Implemented API Functions
//******************************************************************************



void SpmiTarget_PreInit(SpmiDevInfo *pDev)
{
     /* Set TLMM HDRV PULL CTL Setting */
    SPMI_HWIO_OUT(HWIO_TLMM_PMIC_HDRV_PULL_CTL_ADDR, HWIO_TLMM_PMIC_HDRV_PULL_CTL_DATA);
    
    return;
}

