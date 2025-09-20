/*
 * Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
 * Portions Copyright (c) 2008 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef __PLATFORM_INTERRUPTS_H
#define __PLATFORM_INTERRUPTS_H

#include "sys/types.h"

status_t mask_interrupt(unsigned int vector);
status_t unmask_interrupt(unsigned int vector);

typedef enum handler_return (*int_handler)(void *arg);

void register_int_handler(unsigned int vector, int_handler handler, void *arg);

void arm_gic_update_routing (unsigned int intr_num, uint64_t affinity);

enum intr_config
{
   INT_CONFIG_NONE             = 0,
   INT_CONFIG_EDGE_TRIGGER     = 1,
   INT_CONFIG_MAX              = 2,
};

status_t configure_interrupt(unsigned int vector, enum intr_config icfg, void* arg);

#endif
