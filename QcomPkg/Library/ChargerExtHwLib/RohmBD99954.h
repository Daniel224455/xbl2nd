/**
@file  RohmBD99954.h
@brief Provide UsbPwrCtrlLib Rohm Module related definition
*/
/*=============================================================================
Copyright (c) 2017-2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/01/18   mr      Added VIN (VBUS) control register info
12/13/17   dc      Enable Vbus Anti-Collapsing
12/11/17   dc      Enable HW Input Current Limit update based on BC1.2 Detection 
11/09/17   pbitra  Added more commands
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
#define CHGSTM_STATUS_SUSPEND_MASK			0x00
#define CHGSTM_STATUS_TRICKLE_MASK			0x01
#define CHGSTM_STATUS_PRECHARGE_MASK		0x02
#define CHGSTM_STATUS_FAST_MASK				0x03
#define CHGSTM_STATUS_TOPOFF_MASK			0x04
#define CHGSTM_STATUS_DONE_MASK				0x05

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

/* USB VBUS/VCC Input Control Setting */
#define ROHM_VIN_CTRL_SET                   0x0A
#define VBUS_EN_MASK                    0x0040

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
/* Vbus BC1.2 */
#define ROHM_EXT_CMD_CHGOP_SET1         0x0B
#define ILIM_AUTO_DISEN_MASK            0x2000
#define VBUS_BC_DISEN_MASK              0x0400

/* Charger Enable */
#define ROHM_EXT_CMD_CHGOP_SET2         0x0C
#define CHGOP_SET2_CHG_EN_MASK          0x0080
#define CHGOP_SET2_CHG_EN_SHIFT         7

/* VBUS Anti-collapsing Threshold */
#define ROHM_EXT_VBUSCLPS_TH_SET        0x0D

/* Charger WDOG set */
#define ROHM_EXT_CMD_CHGWDT_SET         0x0F
#define CHGWDT_FST_MASK                 0xFF00
#define CHGWDT_PRE_MASK                 0x00FF


/*Threshold voltage from Pre-charging to Fast-charging*/
#define ROHM_EXT_CMD_VSYSREG_SET        0x11

/* Pre-charge current setting */
#define ROHM_EXT_CMD_IPRECH_SET         0x15
#define IPRECH_LIM_MAX                  1024   /* in mA */


/* FCC setting */
#define ROHM_EXT_CMD_ICHG_SET           0x16
#define ICHG_LIM_MAX                    16320   /* in mA */


/* Terminating current setting */
#define ROHM_EXT_CMD_ITERM_SET			0x17
#define ITERM_LIM_MAX                   1024    /* in mA */


/* FVMax Setting */
#define ROHM_EXT_CMD_VFASTCHG_REG_SET1	0x1A
#define FV_LIM_MIN                   	2560    /* in mV */
#define FV_LIM_MAX                   	19200   /* in mV */

/* Vbus */
#define VBUS_UCD_SET                    0x30
#define BCSRETRY_MASK                   0x1000


/* VBAT OVP setting */
#define ROHM_EXT_CMD_VBATOVP_SET		0x1E
#define VBATOVP_MAX                     19200    /* in mV */


/* SAR-ADC Measurement Control Setting */
#define ROHM_EXT_CMD_VM_CTRL_SET		0x40


/* Battery Current Measurement Value */
#define ROHM_EXT_CMD_IBATP_VAL			0x50
#define IBATP_VAL_MASK					0x3FFF

#define ROHM_EXT_CMD_IBATM_VAL			0x52

/* Battery Current Measurement Value */
#define ROHM_EXT_CMD_VBAT_VAL			0x54
#define VBAT_VAL_MASK					0x3FFF

/* 1st level  Interrupt Setting */
#define ROHM_EXT_CMD_INT0_SET			0x68
#define INT0_EN_MASK					0x0001
#define INT3_EN_MASK					0x0008



/* Battery Thermistor Interrupt Setting */
#define ROHM_EXT_CMD_INT3_SET			0x6B
#define INT3_SET_TH_DET_MASK			0x8000


/* Battery status */
#define ROHM_EXT_CMD_INT3_STATUS        0x73
#define INT3_STATUS_TH_DET_MASK         0x8000

#endif  /* __ROHM_BD99956REG_H__ */
