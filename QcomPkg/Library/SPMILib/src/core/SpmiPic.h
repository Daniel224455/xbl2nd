
#ifndef SPMIPIC_H
#define SPMIPIC_H
/**
 * @file:  SpmiPic.h
 * @brief:
 *
 * Copyright (c) 2013-2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $Change: 30389851 $
 *
 *                              Edit History
 * Date     Description
 * -------  -------------------------------------------------------------------
 * 5/01/15  Added API to clear all Interrupts
 * 1/20/15  Multiple bus support
 * 10/1/13  Initial Version
 */


#include "SpmiTypes.h"
#include "SpmiInfo.h"

Spmi_Result SpmiPic_Init(SpmiDevInfo *pDev, uint8 owner);
Spmi_Result SpmiPic_IsInterruptEnabled(SpmiDevInfo *pDev, uint16 channel, boolean* enabled);
Spmi_Result SpmiPic_SetInterruptEnabled(SpmiDevInfo *pDev, uint8 owner, uint16 channel, boolean enabled);
Spmi_Result SpmiPic_InterruptClear(SpmiDevInfo *pDev, uint16 channel, uint32 statusMask);
Spmi_Result SpmiPic_ExtendedInterruptStatus(SpmiDevInfo *pDev, uint16 channel, uint32* statusMask);
Spmi_Result SpmiPic_InterruptClearAll(SpmiDevInfo *pDev, uint8 owner);

#endif

