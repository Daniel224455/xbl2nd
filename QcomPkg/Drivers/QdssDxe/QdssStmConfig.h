/*=============================================================================

FILE:         QdssStmConfig.h

DESCRIPTION:

================================================================================
                Copyright 2015 Qualcomm Technologies Incorporated.
                         All Rights Reserved.
                QUALCOMM Proprietary and Confidential
================================================================================
==============================================================================*/
#ifndef QDSSSTMCONFIG_H_
#define QDSSSTMCONFIG_H_

#include "HALcomdef.h"

void QdssStmConfig_PreInit(uint32 ts_counter_freq);
void QdssStmConfig_Init(void);
void QdssStmConfig_SetTimestampFreq(uint32 dwFreq);

#endif  /* QDSSSTMCONFIG_H_ */
