#ifndef __CCI_HAL_8998_HWIO_H_
#define __CCI_HAL_8998_HWIO_H_

/** @file CCI_HAL_PLATFORM_HWIO.h

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/
    // The max number of i2c masters on our product line
    2,

    // The max number of i2c queues on our product line
    4,

    128,

    32, 

    32, 

    16,

    5,

    // The max number of gpio queues on our product line
    3,
    // The max number of queues on our product line
    7,
    // The max number of sync timer
    4,

    32,

    8,

    0x100, // master offset 
    0x100, // master_base  
    0x100, // queue_offset
    0x300, // queue_base
    0xC00, // irq_base
    0xC04, // irq_set_base
    0x0C,  // irq_set_offset
    0x10,  // sync_timer_base 
    0x04,  // sync_timer_offset
    0x0B,
    0x0A,
    /// mx_read_fifo_len
    0x0F, 
    /// max cid
    16,
    /// default retry count
    3,
    4,
    CAMSENSOR_I2C_400KHZ,
    m0q0,
    gpioq0,
    16,
/// I2C burst mode support
    TRUE, 
/// number of i2c frequency bus speed options
    3,
    { 
///     THIGH, TLOW, TSUSTO, TSUSTA, THDDAT, THDSTA, TBUF, SCL_STRETCH, TRDHLD, TSP, 
        { 104,   88,    105,    119,     13,     84,   91,           0,      6, 3 }, 
//      {  20,   28,     21,     21,     13,     18,   25,           0,      6, 3 }, //400kHz 
        {  20,   29,     21,     21,     13,     18,   25,           0,      6, 3 }, //392kHz 		
        {  16,   22,     17,     18,     16,     15,   19,           0,      3, 3 }
///        {  17,   21,     17,     18,     16,     15,   19,           0,      3, 3 }
    },
    {
        {
            /// master type
            m0,
            /// queue type
            m0q0,
            /// queue start id
            i2c_m0_q0_start,
            /// queue depth
            64,
        },
        {
            /// master type
            m0,
            /// queue type
            m0q1,
            /// queue start id
            i2c_m0_q1_start,
            /// queue depth
            16,
        },
        {
            /// master type
            m1,
            /// queue type
            m1q0,
            /// queue start id
            i2c_m1_q0_start,
            /// queue depth
            64,
        },
        {
            /// master type
            m1,
            /// queue type
            m1q1,
            /// queue start id
            i2c_m1_q1_start,
            /// queue depth
            16,
        },
        {
            /// master type
            gpio,
            /// queue type
            gpioq0,
            /// queue start id
            gpio_q0_start,
            /// queue depth
            32,
        },
        {
            /// master type
            gpio,
            /// queue type
            gpioq1,
            /// queue start id
            gpio_q1_start,
            /// queue depth
            16,
        },
        {
            /// master type
            gpio,
            /// queue type
            gpioq2,
            /// queue start id
            gpio_q2_start,
            /// queue depth
            16,
        },

    }
#endif
