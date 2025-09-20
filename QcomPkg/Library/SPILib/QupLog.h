#ifndef _QUPLOG_H_
#define _QUPLOG_H_
/*
===========================================================================

FILE:   QupLog.h

DESCRIPTION:
    This file contains the API for the QUP Log services 


===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
03/31/14 UR     Added CR to logs
07/10/12 LK     Created

===========================================================================
           Copyright c 2012-2014 Qualcomm Technologies Incorporated.
                              All Rights Reserved.
                           Qualcomm Proprietary/GTDR

===========================================================================
*/
#include <Library/DebugLib.h>

#include "QupTypes.h"

typedef void* QUPLOG_DEV_IMP_HANDLE;

typedef struct QupLogSvcType
{
   QUPLOG_DEV_IMP_HANDLE (*pfnGetReadHandle)(uint32 uDevAddr);
   QUPLOG_DEV_IMP_HANDLE (*pfnGetWriteHandle)(uint32 uDevAddr);
} QupLogSvcType;

#define QUPLOG_LOG_WRITE(address,level,format_str,args...) \
    DEBUG((EFI_D_VERBOSE, format_str"\n", ##args))

#define QUPLOG_LOG_READ(address,level,format_str,args...) \
    DEBUG((EFI_D_VERBOSE, format_str"\n", ##args))

#endif //_QUPLOG_H_

