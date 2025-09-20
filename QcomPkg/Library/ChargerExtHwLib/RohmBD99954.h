/**
@file  RohmBD99954.h
@brief Provide UsbPwrCtrlLib Rohm Module related definition
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
08/28/17   mr      Initial Release
=============================================================================*/
#ifndef __ROHM_BD99956REG_H__
#define __ROHM_BD99956REG_H__


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define ROHM_CMD_SIZE                       1   /* 1Byte */
#define ROHM_DATA_SIZE                      2   /* 1Word */

/** ROHM COMMAND TYPE SELECT */
#define ROHM_CMD_SEL_MAP_SET                0x3F

/** ROHM BATTERY CHARGER COMMANDS */


/** ROHM EXTENDED COMMANDS */

/* Charger State Machine Status */
#define ROHM_EXT_CMD_CHGSTM_STATUS          0x00

/* VBAT and VSYS Status */
#define ROHM_EXT_CMD_VBATT_VSYS_STATUS      0x01

/* VBUS and VCC Status */
#define ROHM_EXT_CMD_VBUS_VCC_STATUS        0x02
#define VBUS_VCC_STS_VBUS_DET           0x0001
#define VBUS_VCC_STS_VBUS_CLPS          0x0002
#define VBUS_VCC_STS_ILIM_VBUS_MOD      0x0004
#define VBUS_VCC_STS_VBUS_OVP           0x0008
#define VBUS_VCC_STS_VCC_DET            0x0100
#define VBUS_VCC_STS_VCC_CLPS           0x0200
#define VBUS_VCC_STS_ILIM_VCC_MOD       0x0400
#define VBUS_VCC_STS_VCC_OVP            0x0800
#define VBUS_VCC_STS_VACP_DET           0x1000

/* Charger Operation Status */
#define ROHM_EXT_CMD_CHGOP_STATUS           0x03

/* Charger WDT and Thermal WDT Status */
#define ROHM_EXT_CMD_WDT_STATUS             0x04

/* Actual Input Current Limit */
#define ROHM_EXT_CMD_CURRENT_ILIM_VAL       0x05

/* Selected Input Current Limit */
#define ROHM_EXT_CMD_SEL_ILIM_VAL           0x06

/* USB VBUS Input Current Limit Setting */
#define ROHM_EXT_CMD_IBUS_LIM_SET           0x07
#define IBUS_LIM_SET_MASK               0x3FE0
#define IBUS_LIM_STEP                   32      /* in mA */
#define IBUS_LIM_MAX                    16352   /* in mA */

/* BC1.2 Charger (Port) Detect Status on the VBUS side */
#define ROHM_EXT_CMD_VBUS_UCD_STATUS        0x31
#define VBUS_UCD_STS_OTGDET             0x0008
#define VBUS_UCD_STS_CHGDET             0x0040
#define VBUS_UCD_STS_VBUS_VLD           0x0080
#define VBUS_UCD_STS_PUPDET             0x0800
#define VBUS_UCD_STS_CHGPORT0           0x1000
#define VBUS_UCD_STS_CHGPORT1           0x2000
#define VBUS_UCD_STS_DCDFAIL            0x8000

#define VBUS_UCD_STS_PORT_DET_MASK      (VBUS_UCD_STS_CHGDET    | \
                                         VBUS_UCD_STS_PUPDET    | \
                                         VBUS_UCD_STS_DCDFAIL   | \
                                         VBUS_UCD_STS_CHGPORT0  | \
                                         VBUS_UCD_STS_CHGPORT1)
#define VBUS_UCD_STS_PORT_SDP           (VBUS_UCD_STS_CHGPORT0)
#define VBUS_UCD_STS_PORT_CDP           (VBUS_UCD_STS_CHGDET    | \
                                         VBUS_UCD_STS_CHGPORT1)
#define VBUS_UCD_STS_PORT_DCP           (VBUS_UCD_STS_CHGDET    | \
                                         VBUS_UCD_STS_CHGPORT0  | \
                                         VBUS_UCD_STS_CHGPORT1)
#define VBUS_UCD_STS_PORT_PULLUP        (VBUS_UCD_STS_PUPDET    | \
                                         VBUS_UCD_STS_DCDFAIL   | \
                                         VBUS_UCD_STS_CHGPORT0)
#define VBUS_UCD_STS_PORT_OPEN          (VBUS_UCD_STS_DCDFAIL   | \
                                         VBUS_UCD_STS_CHGPORT0)
#define VBUS_UCD_STS_PORT_UNSTABLE      (VBUS_UCD_STS_DCDFAIL   | \
                                         VBUS_UCD_STS_CHGPORT0)


#endif  /* __ROHM_BD99956REG_H__ */
