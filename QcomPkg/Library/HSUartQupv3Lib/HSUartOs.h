#ifndef HSUART_OS_H
#define HSUART_OS_H
/*==================================================================================================

FILE: uart_os.h

DESCRIPTION: This module provides the OS specific APIs to the UART driver software.

Copyright (c) 2018 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
11/16/18   NM       Initial revision

==================================================================================================*/
/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include "HSUartApi.h"
#include "HSUartDefs.h"

void        hsuart_busy_wait(uint32 uSecs);
hsuart_result hsuart_clock_close(hsuart_context* h);
hsuart_result hsuart_clock_open(hsuart_context* h,uint32 input_freq);
hsuart_result hsuart_get_driver_properties(void);
hsuart_result hsuart_get_properties(hsuart_context* h);
hsuart_result hsuart_interrupt_close(hsuart_context* h);
hsuart_result hsuart_interrupt_done(hsuart_context* h);
hsuart_result hsuart_interrupt_open(hsuart_context* h, void* isr);
hsuart_result hsuart_tlmm_open(hsuart_context* h);
hsuart_result hsuart_tlmm_close(hsuart_context* h);

#endif

