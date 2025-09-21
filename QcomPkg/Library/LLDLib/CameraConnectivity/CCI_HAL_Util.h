#ifndef _H_CCI_HAL_Util_H_
#define _H_CCI_HAL_Util_H_

/** @file CCI_HAL_Util.h

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/

#include "CameraCCITypes.h"
#include "CCI_HAL_HWIO.h"

typedef enum 
{
    signal_0 = 0,
    signal   = signal_0,
    signal_async,
    signal_async_thread_evt,
    signal_async_thread_abort,
    signal_async_proc_started,
    signal_async_i2c_complete,
    signal_max
} CCI_SIGS;

#endif
