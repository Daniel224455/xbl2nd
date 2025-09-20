/**
@file  UsbPwrCtrlLibComm.c
@brief Implementation of I2C interface APIs for External Charger Module
*/
/*=============================================================================
Copyright (c) 2017 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
08/10/17   mr      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"


/*===========================================================================*/
/*                  FUNCTION DEFINITIONS                                     */
/*===========================================================================*/
/**
 * I2C Read/Write Interface for External Charger Hw
 */
i2c_status UsbPwrCtrl_i2c_read(UsbPwrCtrl_Device *I2CDev, UINT8 offset, UINT8 *data_out, UINT16 size_out)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 read_cnt = 0;

    if(I2CDev && I2CDev->hI2CHandle && data_out)
    {
        i2c_sts = i2c_read(I2CDev->hI2CHandle,
                            &I2CDev->I2CSlaveCfg,
                            (uint16)offset,
                            I2CDev->I2CSlaveRegMap,
                            data_out,
                            size_out,
                            &read_cnt,
                            I2CDev->TimeoutMs);

        // if(read_cnt != size_out)  {i2c_sts = I2C_TRANSFER_INVALID;}
    }
    else
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;

    return i2c_sts;
}

i2c_status UsbPwrCtrl_i2c_write(UsbPwrCtrl_Device *I2CDev, UINT8 offset, UINT8 *data_in, UINT16 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 write_cnt = 0;

    if(I2CDev && I2CDev->hI2CHandle && data_in)
    {
        i2c_sts = i2c_write(I2CDev->hI2CHandle,
                            &I2CDev->I2CSlaveCfg,
                            (uint16)offset,
                            I2CDev->I2CSlaveRegMap,
                            data_in,
                            size_in,
                            &write_cnt,
                            I2CDev->TimeoutMs);

        // if(write_cnt != size_in) {i2c_sts = I2C_TRANSFER_INVALID;}
    }
    else
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;

    return i2c_sts;
}

