#ifndef __CAMERAPLATFORM_H_
#define __CAMERAPLATFORM_H_

/* ===========================================================================

              C a m e r a P l a t f o r m   D e c l a r a t i o n s

*//** @file CameraPlatform.h
@brief Header file for CameraPlatform interface.

This header file defines the interface for CameraPlatform which abstracts the
BSP from the camera components that use it.

Copyright (c) 2007-2017 Qualcomm Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*//*======================================================================= */

/*============================================================================

                             Edit History
 when      who    what, where, why
 --------  -----  ----------------------------------------------------------
 09/13/07  dl     Created

============================================================================*/


typedef enum
{
    CAMSENSOR_I2C_MIN       = 100,
    CAMSENSOR_I2C_100KHZ    = CAMSENSOR_I2C_MIN,
    CAMSENSOR_I2C_400KHZ    = 400,
    CAMSENSOR_I2C_1000KHZ   = 1000,
    CAMSENSOR_I2C_3400KHZ   = 3400,
    CAMSENSOR_I2C_MAX,
} CameraSensorI2C_SpeedType;


typedef enum
{
    /// Use default address for I/O
    CAMSENSOR_I2C_SIMPLE_DEV = 0x0001,
    /// memory device (16 bit addresses)
    CAMSENSOR_I2C_MEM_DEV = 0x0002,
    /// 8-bit register based device
    CAMSENSOR_I2C_REG_DEV = 0x0004,
    /// NACK last byte read from slave
    CAMSENSOR_I2C_NACK_LAST_BYTE = 0x0008,
    /// Let slave release SDA after last byte is read from it.
    CAMSENSOR_I2C_NOP_LAST_BYTE = 0x0010,
    /// During a read transaction, gen. a repeated START after writing the slave addr(just before read)
    CAMSENSOR_I2C_START_BEFORE_READ = 0x0020,
    /// During a read transaction, gen. a STOP & START after writing the slave addr (just before read)
    CAMSENSOR_I2C_STOP_START_BEFORE_READ = 0x0040,
    /// Generate a clock and a START before every I/O operation.
    CAMSENSOR_I2C_CLK_START_BEFORE_RW = 0x0080,
    /// No stop condition after writing the address
    CAMSENSOR_I2C_NO_STOP_AFTER_WRITE = 0x0100,
    /// Immediatly read the data
    CAMSENSOR_I2C_IMMEDIATE_READ = 0x0200,
    /// Delay periods for for HW CTRL
    CAMSENSOR_I2C_USE_DELAY_PERIODS   = 0x0400,
    /// Write Multiple I2C Sequentially
    CAMSENSOR_I2C_SEQ_WR = 0x0800,
    /// Collapses singles i2c to burst mode if possible 
    CAMSENSOR_I2C_COLLAPSIBLE_SEQ_WR = 0x1000,
    /// Overrides existing burst mode setting and enable burst mode
    CAMSENSOR_I2C_FORCED_BURST_MODE = 0x2000,
} CameraSensorI2COptionsType;

typedef enum
{
    CAMSENSOR_EXP_CALLBACK = 1,
    CAMSENSOR_FCS_CALLBACK,
    CAMSENSOR_FLSH_CALLBACK,
    CAMSENSOR_ASYNC_I2C_CALLBACK,
    CAMSENSOR_GPIO_IN_CALLBACK,
    CAMSENSOR_MX_CALLBACK
} CameraSensorCallbackType;


typedef enum
{
    INVALID_CHIP_ID,
    MSM7x27,
    MSM7x27A,
    MSM7x30_V1,
    MSM7x30_V2,
    QSD8650A,
    QSD8650,
    MSM8660_V1,
    MSM8660_V2,
    MSM8930_V3,
    MSM8960_V1,
    MSM8960_V2,
    MSM8909,
    MSM8930,
    MSM8610,
    MSM8974,
    MSM8626,
    MSM8916,
    APQ8016,
    APQ8092,
    MSM8992,
    MSM8994,
    MSM8996,
    APQ8096,
    APQ8052,
    MSM8952,
	APQ8053,
	MSM8953,
    MSM8998,
    SDM845,
} CameraPlatformChipIdType;

typedef struct CameraCameraSensorI2CRegConfigType 
{
    UINT16 RegAddr;
    UINT16 RegData;
    UINT32 delay;
} CameraSensorI2CRegConfigType;

#endif // __CAMERAPLATFORM_H_
