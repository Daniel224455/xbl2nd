/*! \file pm_smb_common.c
*  \n
*  \details This file contains the common APIs for SMB chipsets.
*
*  \n &copy; Copyright 2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/04/17   pxm     Created
===========================================================================*/

/*===========================================================================
INCLUDE FILES
===========================================================================*/
#include "pm_utils.h"
#include "CoreVerify.h"
#include "pm_smb.h"

#define SMB_TIMEOUT_MAX 2500
#define SMB_BUS_FREQUENCY_KHZ 400
#define SMB_MAX_CLOCK_STRETCH_US 500
// SMB1355 mounted on I2C instance 010, but since SBL sequence start from 1, 
// so here it is 011 instead 010
#define SMB_I2C_INSTANCE_DEFAULT I2C_INSTANCE_011
#define SMB_SLAVE_ADDRESS_DEFAULT 0x08

// Allow 3 times retry for the scenario of slave NACKED.
#define I2C_SLAVE_NACKED (1 << 3)
#define SMB_RETRY_COUNT_MAX 3

#define ADDRESS_PERPH_TYPE 0x104
#define ADDRESS_PERPH_SUBTYPE 0x105
#define PERPH_TYPE_SMB 0x51
#define PERPH_SUBTYPE_SMB1355 0x1C
#define PERPH_SUBTYPE_SMB138x 0x17


// TODO: Need to handle the case that instance number and slave address
// are different with reference design case. Or slave address may vary
// for different CSIR.
// Considering add a slave address list in pm_config_target.c. Go through
// on this address, reading type/subtype to check whether it exist, if exist,
// get the right slave address.


static pm_smb_data_type *smb = NULL;

pm_err_flag_type tadc_leakage_wa(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if(NULL != smb && NULL != smb->tadc_leakage_wa)
    {
        err_flag = smb->tadc_leakage_wa();
    }

    return err_flag;
}

pm_err_flag_type resemble_csir(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if(NULL != smb && NULL != smb->resemble_csir)
    {
        err_flag = smb->resemble_csir();
    }

    return err_flag;
}


pm_err_flag_type pm_smb_init(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    i2c_status status = I2C_SUCCESS;
    uint8 data = 0;
    uint8 retry_idx = 0;

    if(NULL == smb)
    {
        pm_malloc(sizeof(pm_smb_data_type), (void**)&smb);
        CORE_VERIFY(NULL != smb);
    }

    smb->opened = FALSE;
    smb->i2c_handle = NULL;
    smb->instance = SMB_I2C_INSTANCE_DEFAULT;
    smb->pm_smb_name = PM_SMB_NAME_INVALID;

    smb->config.bus_frequency_khz = SMB_BUS_FREQUENCY_KHZ;
    smb->config.slave_max_clock_stretch_us = SMB_MAX_CLOCK_STRETCH_US;
    smb->config.slave_address = SMB_SLAVE_ADDRESS_DEFAULT;
    smb->config.mode = I2C;
    smb->config.core_configuration1 = 0;
    smb->config.core_configuration2 = 0;

    status = i2c_open(smb->instance, &smb->i2c_handle);
    if(I2C_SUCCESS != status) return PM_ERR_FLAG_BUS_ERR;

    // For the first time access, allow at most 3 times retry. workaround for hw
    for(retry_idx = 0; retry_idx < SMB_RETRY_COUNT_MAX; ++retry_idx)
    {
        status = smb_read(ADDRESS_PERPH_TYPE, &data);
        if(I2C_SUCCESS == status
            || (I2C_SLAVE_NACKED != (I2C_SLAVE_NACKED & status)))
        {
            // Break if read success or failure but not NACKED
            break;
        }
    }

    if(I2C_SUCCESS == status)
    {
        if(PERPH_TYPE_SMB == data)
        {
            status = smb_read(ADDRESS_PERPH_SUBTYPE, &data);
            if(I2C_SUCCESS == status)
            {
                switch(data)
                {
                    case PERPH_SUBTYPE_SMB1355:
                        smb->pm_smb_name = PM_SMB_NAME_1355;
                        pm_smb_function_pointer_assign_1355(smb);
                        break;
                    case PERPH_SUBTYPE_SMB138x:
                        smb->pm_smb_name = PM_SMB_NAME_138x;
                        pm_smb_function_pointer_assign_138x(smb);
                        break;
                    default:
                        smb->pm_smb_name = PM_SMB_NAME_INVALID;
                        break;
                }

                if(PM_SMB_NAME_INVALID != smb->pm_smb_name)
                {
                    smb->opened = TRUE;
                }
            }
        }
    }

    if(I2C_SUCCESS != status)
    {
        err_flag = PM_ERR_FLAG_BUS_ERR;
    }

    return err_flag;
}

void pm_smb_set_instance(i2c_instance instance)
{
    if(NULL != smb)
    {
        smb->instance = instance;
    }
}

void pm_smb_set_slave_address(uint32 address)
{
    if(NULL != smb)
    {
        smb->config.slave_address = address;
    }
}

pm_err_flag_type pm_smb_deinit(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    i2c_status status = I2C_SUCCESS;

    if(NULL != smb && smb->opened)
    {
        status = i2c_close(smb->i2c_handle);
        if(I2C_SUCCESS != status)
        {
            err_flag = PM_ERR_FLAG_BUS_ERR;
        }
    }

    return err_flag;
}

pm_smb_data_type* pm_smb_get_data(void)
{
    return smb;
}

pm_smb_name_type pm_smb_get_chip_name()
{
    return (smb == NULL) ? PM_SMB_NAME_INVALID : smb->pm_smb_name;
}

i2c_status smb_read(uint16 offset, uint8 *buffer)
{
    i2c_status status = I2C_SUCCESS;
    uint32 read_count = 0;

    if((NULL == buffer) || (NULL == smb) || !smb->opened)
    {
        return status;
    }

    status = i2c_read(smb->i2c_handle, &(smb->config), offset, 2, buffer,
                        1, &read_count, SMB_TIMEOUT_MAX);

    return status;
}

i2c_status smb_write(uint16 offset, uint8 data)
{
    i2c_status status = I2C_SUCCESS;
    uint32 written_count = 0;

    
    if((NULL == smb) || !smb->opened)
    {
        return status;
    }

    status = i2c_write(smb->i2c_handle, &(smb->config), offset, 2, &data,
                        1, &written_count, SMB_TIMEOUT_MAX);

    return status;
}


