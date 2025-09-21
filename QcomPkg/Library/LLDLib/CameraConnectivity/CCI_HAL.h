#ifndef H_CCI_HAL_H
#define H_CCI_HAL_H

/** @file CCI_HAL.h

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/

#include "CameraPlatform.h"
#include "CameraCCITypes.h"
#include "CCI_HAL_Util.h"
#include "CCIResourceType.h"

CCIStatus CCI_HAL_get_queueInfo(QueueInfo* queueInfo, CCICommandQueueType eQueueType);
CCIStatus CCI_HAL_Read_I2C_Data(QueueInfo queueInfo, UINT8* buff, UINT32 len, CCIStatus *eStatus);
CCIStatus CCI_HAL_SetFrequency(CCIMasterType masterId, CameraSensorI2C_SpeedType efrequency);

/* ---------------------------------------------------------------------------
*    FUNCTION        CCI_HAL_Get_CTRLReg
*    DESCRIPTION     returns pointer to CCI Control registers 
*    DEPENDENCIES
*    PARAMETERS      None 
*    RETURN VALUE    returns pointer to CCI Control registers 
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */
CCIHWCTRL CCI_HAL_Get_CTRLReg(void);

CCIStatus CCI_HAL_UEFI_Init(void);


#endif 

