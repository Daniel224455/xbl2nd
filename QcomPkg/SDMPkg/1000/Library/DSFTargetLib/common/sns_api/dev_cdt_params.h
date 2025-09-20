#ifndef __DEV_CDT_PARAMS_H__
#define __DEV_CDT_PARAMS_H__

/*=============================================================================

                                DDR Device CDT PARAMETERS
                                Header File
GENERAL DESCRIPTION
This file defines the DDR paramters that is stored/retrieved from CDT partition
=============================================================================*/
/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2013-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
/*==========================================================================

                               INCLUDE FILES
===========================================================================*/
#include "HALcomdef.h"

/*==============================================================================
                                  TYPES
==============================================================================*/

/**************************/
/*** CDT DDR Parameters ***/
/**************************/

struct detected_ddr_device_params
{
  uint8 manufacturer_id;
  uint8 device_type;

  uint8 num_rows[2];
  uint8 num_cols[2];
  uint8 num_banks[2];
  uint8 device_io_width[2];
  uint8 device_density[2];
  uint8 device_density_in_Gb[2];
  uint8 revision_id_1[2];
  uint8 revision_id_2[2];
  
  uint8 populated_chipselect;
};

#endif /* __DEV_CDT_PARAMS_H__ */
