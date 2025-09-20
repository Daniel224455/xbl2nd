/*! \file pm_smb138x.c 
*  \n
*  \brief This file contains SMB138x dedicate funciton definition.
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
//pm_err_flag_type (*tadc_leakage_wa_138x)(void);

pm_err_flag_type tadc_leakage_wa_138x(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    i2c_status status = I2C_SUCCESS;

    status |= smb_write(0x36D0, 0xA5);
    status |= smb_write(0x3652, 0x00);
    status |= smb_write(0x36D0, 0xA5);
    status |= smb_write(0x36E7, 0x80);

    if(I2C_SUCCESS != I2C_SUCCESS)
    {
        err_flag = PM_ERR_FLAG_BUS_ERR;
    }

    return err_flag;
}

void pm_smb_function_pointer_assign_138x(pm_smb_data_type* smb)
{
    smb->resemble_csir = NULL; // only applicable for SMB1355
    smb->tadc_leakage_wa = tadc_leakage_wa_138x;    // only applicable for SMB1381/0
}



