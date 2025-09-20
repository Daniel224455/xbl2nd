/*! \file pm_smb1355.c 
*  \n
*  \brief This file contains SMB1355 dedicate funciton definition.
*  \n
*  \n &copy; Copyright 2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/05/17   pxm     Creation
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_smb.h"
/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/



/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
//pm_err_flag_type (*resemble_csir_1355)(void);

pm_err_flag_type resemble_csir_1355(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    i2c_status status = I2C_SUCCESS;

    // These sequence will resemble CSIR
    status |= smb_write(0x1042, 0x00);
    status |= smb_write(0x1051, 0x01);
    status |= smb_write(0x1651, 0x00);

    if(I2C_SUCCESS != I2C_SUCCESS)
    {
        err_flag = PM_ERR_FLAG_BUS_ERR;
    }

    return err_flag;
}

void pm_smb_function_pointer_assign_1355(pm_smb_data_type* smb)
{
    smb->resemble_csir = resemble_csir_1355; // only applicable for SMB1355
    smb->tadc_leakage_wa = NULL; // only applicable for SMB1381/0
}


