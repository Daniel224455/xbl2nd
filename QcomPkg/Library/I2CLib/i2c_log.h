/** 
    @file  i2c_log.h
    @brief I2C logging interface
 */
/*=============================================================================
            Copyright (c) 2017-2018 Qualcomm Technologies, Incorporated.
                              All rights reserved.
              Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

#ifndef __I2C_LOG_H__
#define __I2C_LOG_H__

#include<Library/DebugLib.h>

//#define I2C_LOG_ENABLE_INFO
#define I2C_LOG_ENABLE_ERROR

// limit arguments to 10
#define I2C_LOG_NUM_ARGS(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) (N - 1)
#define I2C_LOG(level, msg, args...) I2C_LOG_##level(msg, ##args)

#ifdef I2C_LOG_ENABLE_DATA
#define I2C_LOG_LEVEL_DATA(msg, args...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_DATA(msg, args...)
#endif

#ifdef I2C_LOG_ENABLE_ERROR
#define I2C_LOG_LEVEL_ERROR(msg, args...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_ERROR(msg, args...)
#endif

#ifdef I2C_LOG_ENABLE_INFO
#define I2C_LOG_LEVEL_INFO(msg, args...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_INFO(msg, args...)
#endif

#ifdef I2C_LOG_ENABLE_REGS
#define I2C_LOG_LEVEL_REGS(msg, args...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_REGS(msg, args...)
#endif

#ifdef I2C_LOG_ENABLE_PERF
#define I2C_LOG_LEVEL_PERF(...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_PERF(...)
#endif

#ifdef I2C_LOG_ENABLE_VERBOSE
#define I2C_LOG_LEVEL_VERBOSE(...) DEBUG((EFI_D_ERROR, msg"\n", ##args))
#else
#define I2C_LOG_LEVEL_VERBOSE(...)
#endif

#endif
