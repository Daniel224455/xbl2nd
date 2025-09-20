#ifndef __PM_SMB_DRIVER_H__
#define __PM_SMB_DRIVER_H__

/*! \file pm_smb_driver.h 
*  \n
*  \brief  This file contains functions prototypes and variable/type/constant
*         declarations for supporting smb chipsets.
*  
*  \n &copy; Copyright 2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/04/17   pxm     Created
========================================================================== */

/*===========================================================================
						INCLUDE FILES
===========================================================================*/
#include "i2c_api.h"
#include "pm_err_flags.h"


/*===========================================================================

                     SMB TYPES AND STRUCTURES 

===========================================================================*/

typedef enum
{
    PM_SMB_NAME_INVALID,
    PM_SMB_NAME_138x,
    PM_SMB_NAME_1355
} pm_smb_name_type;


typedef struct
{
    // member variables
    i2c_instance instance;
    i2c_slave_config config;
    boolean opened;
    void *i2c_handle;
    pm_smb_name_type pm_smb_name; // Identified by type:subtype pair.

    // member function pointers
    pm_err_flag_type (*tadc_leakage_wa)(void);
    pm_err_flag_type (*resemble_csir)(void);
} pm_smb_data_type;

/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/

/* 
 * All SMB chipset APIs have dependency with pm_smb_init, make sure you called 
 * this function before you want to manupilate on SMB chipset, includeing smb 
 * read/write.
 */
pm_err_flag_type pm_smb_init(void);

pm_err_flag_type pm_smb_deinit(void);

/*
 * Work around required for SMB1381/0 for tadc leakage issue.
 */
pm_err_flag_type tadc_leakage_wa(void);

/*
 * Resemble CSIR. Required by SMB1355 as a workaround to disable charge during 
 * boot.
 */
pm_err_flag_type resemble_csir(void);

/*
 * return global struct pm_smb_data_type
 */
pm_smb_data_type* pm_smb_get_data(void);

/*
 * This function return enum SMB chip name.
 */
pm_smb_name_type pm_smb_get_chip_name(void);

/*
 * Set i2c instance number if default instance is not the one actually used.
 * @param instance [in]: new instance number which will be used. need to check 
 *                       whether the sequence is base on 0 or 1.
 */
void pm_smb_set_instance(i2c_instance instance);

/*
 * Set i2c slave address if default instance is not the one actually used.
 * @param address [in]: new slave address which will be used for register 
 *                       access. This is the 7-bit address.
 */
void pm_smb_set_slave_address(uint32 address);

/*
 * assign function pointers of 138x to struct pm_smb_data_type. Some APIs are 
 * not applicable for all SMB chipsets hence we use function pointer to handle
 * the difference in different SMB chipset.
 */
void pm_smb_function_pointer_assign_138x(pm_smb_data_type* smb);

/*
 * assign function pointers of 1355 to struct pm_smb_data_type. Some APIs are 
 * not applicable for all SMB chipsets hence we use function pointer to handle
 * the difference in different SMB chipset.
 */
void pm_smb_function_pointer_assign_1355(pm_smb_data_type* smb);

/*
 * Read a byte  from SMB chipset at given offset.
 * Dependency: pm_smb_init need to be successfully called in advance.
 * @param offset [in]: 2 bytes offset where the data need to read.
 * @param buffer [out]: the buffer used to carry out the data. user should 
 *                      allocate memory for buffer itself.
 * @return I2C_SUCCESS if success or variable not well initialized.
 *         otherwize: error code
 */
i2c_status smb_read(uint16 offset, uint8 *buffer);

/*
 * write a byte to SMB chipset at given offset.
 * Dependency: pm_smb_init need to be successfully called in advance.
 * @param offset [in]: 2 bytes offset where the data need to write.
 * @param data [in]: the data which will be written.
 * @return I2C_SUCCESS if success or variable not well initialized.
 *         otherwize: error code
 */
i2c_status smb_write(uint16 offset, uint8 data);

#endif // __PM_SMB_DRIVER_H__

