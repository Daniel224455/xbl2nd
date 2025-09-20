#ifndef __SPINOR_SPIAL__H
#define __SPINOR_SPIAL__H
/**********************************************************************
 *spinor_spial.h
 *
 * This file provides an interface to spi buses driver 
 *
 * Copyright (c) 2017
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*======================================================================

                        EDIT HISTORY FOR MODULE


when         who      what, where, why
----------   ------   ----------------------------------------------- 
2017-05-25  md        Initial Release

==================================================================*/
#include "quadspi.h"


#define SPINOR_SPI_DONE                  0  /**< Operation passed */
#define SPINOR_SPI_FAIL                (-1) /**< Operation failed */
#define SPINOR_SPI_NOT_SUPPORTED       (-2) /**< Device/operation not supported */
#define SPINOR_SPI_INVALID_PARAMETER   (-3) /**< API parameters invalid */


typedef int SPINOR_SPI_STATUS;

typedef enum
{
   qspi_read_mode = 1,
   qspi_write_mode = 2,
   qspi_erase_mode = 3,
   qspi_erase_bulk_mode = 5, 
   qspi_write_enable_mode = 6,
   qspi_register_read = 7,
   qspi_register_write = 8,
} SPI_COMMAND_TYPE;

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

SPINOR_SPI_STATUS spial_init(bool ini_status);
SPINOR_SPI_STATUS spial_deinit(void *handle);
SPINOR_SPI_STATUS spial_transfer(SPI_COMMAND_TYPE cmd_mode, uint32_t addr, uint8 *reg_value, uint32 len, uint8_t opcode );




#endif /* __SPINOR_SPIAL__H */
