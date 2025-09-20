#ifndef _HAL_DP_I_H
#define _HAL_DP_I_H
/*=============================================================================
 
  File: hal_dp_i.h
 
  Internal header file for DP HAL layer 
   
  Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "hal_dp.h"
#include "mdsshwio.h"
#include "dp_system.h"

/* --------------------------------------------------------------------------------------------------------------------
** Preprocessor Definitions and Constants
** ----------------------------------------------------------------------------------------------------------------- */

/* HW values for different lane mapping */
typedef enum
{
   HWIO_DP_LANEMAP_TYPE_NONE                      = 0x00000000,
   HWIO_DP_LANEMAP_TYPE_0123                      = 0xE4,
   HWIO_DP_LANEMAP_TYPE_0132                      = 0xB4,
   HWIO_DP_LANEMAP_TYPE_0213                      = 0xD8,
   HWIO_DP_LANEMAP_TYPE_0231                      = 0x74,
   HWIO_DP_LANEMAP_TYPE_0312                      = 0x9C,
   HWIO_DP_LANEMAP_TYPE_0321                      = 0x6C,
   HWIO_DP_LANEMAP_TYPE_1023                      = 0xE1,
   HWIO_DP_LANEMAP_TYPE_1032                      = 0xB1,
   HWIO_DP_LANEMAP_TYPE_1203                      = 0xC9,
   HWIO_DP_LANEMAP_TYPE_1230                      = 0x39,
   HWIO_DP_LANEMAP_TYPE_1302                      = 0x8D,
   HWIO_DP_LANEMAP_TYPE_1320                      = 0x2D,
   HWIO_DP_LANEMAP_TYPE_2013                      = 0xD2,
   HWIO_DP_LANEMAP_TYPE_2031                      = 0x72,
   HWIO_DP_LANEMAP_TYPE_2103                      = 0xC6 ,
   HWIO_DP_LANEMAP_TYPE_2130                      = 0x36,
   HWIO_DP_LANEMAP_TYPE_2301                      = 0x4E,
   HWIO_DP_LANEMAP_TYPE_2310                      = 0x1E, 
   HWIO_DP_LANEMAP_TYPE_3012                      = 0x93,
   HWIO_DP_LANEMAP_TYPE_3021                      = 0x63,
   HWIO_DP_LANEMAP_TYPE_3102                      = 0x87,
   HWIO_DP_LANEMAP_TYPE_3120                      = 0x27,
   HWIO_DP_LANEMAP_TYPE_3201                      = 0x4B,
   HWIO_DP_LANEMAP_TYPE_3210                      = 0x1B,
   HWIO_DP_LANEMAP_TYPE_MAX,
   HWIO_DP_LANEMAP_TYPE_FORCE_32BIT               = 0x7FFFFFFF
}HWIO_DP_LaneMap_Type;


/* HW values for different RGB color mapping */
typedef enum
{
   HWIO_DP_RGBMAP_TYPE_NONE                       = 0x00000000,
   HWIO_DP_RGBMAP_TYPE_RGB                        = 0x6,
   HWIO_DP_RGBMAP_TYPE_RBG                        = 0x12,
   HWIO_DP_RGBMAP_TYPE_GBR                        = 0x21,
   HWIO_DP_RGBMAP_TYPE_GRB                        = 0x9,
   HWIO_DP_RGBMAP_TYPE_BGR                        = 0x24,
   HWIO_DP_RGBMAP_TYPE_BRG                        = 0x18,
   HWIO_DP_RGBMAP_TYPE_MAX,
   HWIO_DP_RGBMAP_TYPE_FORCE_32BIT                = 0x7FFFFFFF
} HWIO_DP_RGBMapType;

/* HW values for HPD status. */
typedef enum
{
   HWIO_DP_HPD_STATE_STATUS_DISCONNECTED           = 0,
   HWIO_DP_HPD_STATE_STATUS_CONNECT_PENDING        = 1,
   HWIO_DP_HPD_STATE_STATUS_CONNECTED              = 2,
   HWIO_DP_HPD_STATE_STATUS_HPD_IO_GLITCH_COUNT    = 3,
   HWIO_DP_HPD_STATE_STATUS_IRQ_HPD_PULSE_COUNT    = 4,
   HWIO_DP_HPD_STATE_STATUS_HPD_REPLUG_COUNT       = 5,
   HWIO_EDP_LANEMAP_TYPE_MAX,
} HWIO_DP_HPD_Status_Type;

/* The length of AUX CMD FIFO */
#define HAL_DP_AUX_CMD_FIFO_LEN                   144

/* MAX length of a AUX Command */
#define HAL_DP_AUX_CMD_MAX                        16

/* MAX length of a I2C Command */
#define HAL_DP_AUX_CMD_I2C_MAX                    128

/* Define the global power on setting for DP_PHY_DPPHY_GLB_PD_CTL */
#define HAL_DP_GLB_POWERON                        0x3F

/* Define the utility macro, to return length of an array. */
#define HAL_DP_LIST_LENGTH(_LIST_)                ((sizeof(_LIST_))/(sizeof(_LIST_[0])))

/* LUT to convert the HAL_DP_LaneMapType to HWIO_DP_LaneMap_Type */
extern HWIO_DP_LaneMap_Type eHalLaneMapToHWIOLaneMap[];

#endif // _HAL_DP_I_H
