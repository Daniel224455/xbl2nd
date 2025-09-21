
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/
///******************************************************************************
//The contents of this file are provided under the conditions defined
//by the LICENSE.txt.
//Copyright (c) 2017, Analogix Semiconductor, Inc.
///******************************************************************************

#ifndef __ANX74XXLIBREG_H__
#define __ANX74XXLIBREG_H__

#define ANX_REG_ADDR_SIZE  1   /* 1Byte */
#define ANX_DATA_SIZE      1   /* 1Byte */

//slave addr: 0x7e, 0x6e, 0x64,0x62 (divide by 2)
#define OCM_VERSION 0xB4
#define HW_CHIP_REV 0x95  /* (actual reg name) ADDR_DEVICE_ANALOG_VERSION */
#define EFUSE_READ_DATA_0 0x8D  /* (actual reg name) EFUSE_CTRL_15 */
#define IRQ_BITS_MASK_OFFSET 0xB6
#define IRQ_STATUS_OFFSET 0xB7
#define SYSTEM_STATUS_OFFSET 0xB8
#define CC_STATUS_OFFSET 0xB9
#define CONFIG_RDO_MAX_VOLTAGE_OFFSET 0xAC
#define CONFIG_RDO_MAX_POWER_OFFSET 0xAD
#define CONFIG_RDO_MIN_POWER_OFFSET 0xAE
#define RDO_MAX_VOLT_OFFSET 0xAF
#define RDO_MAX_POWER_OFFSET 0xB0
#define D_EXT_MSG_CTRL_OFFSET 0xBB
#define UNSTRUCT_VDM_EN_OFFSET 0xF0

#define TX_BUFFER_OFFSET 0xC0
#define R_RAM_CTRL 0x05
#define OCM_CTRL_0 0x6E

//slave addr: 0x58, 0x54 (divide by 2)
#define COMMAND 0x23
#define FIRMWARE_CTRL 0xB2
#define ROLE_CONTROL 0x1A
#define ANALOG_CTRL_1 0xA1
#define ANALOG_CTRL_6  0xA6
#define ANALOG_CTRL_10 0xAA
#define ADC_CTRL_2 0xC0
#define INTR_ALERT_1_MASK 0xCA
#define INTR_ALERT_0 0xCB
#define INTR_ALERT_1 0xCC
#define RING_OSC_CTRL 0xD9


/* ANX TCPC Chip Revision */
#define AA_CHIP_REV 0xAA
#define AC_CHIP_REV 0xAC

//Bits definition
#define VCONN_STATUS 0x04
/*0: vbus off; 1: vbus on*/
#define VBUS_STATUS 0x08
/*0: host; 1:device*/
#define DATA_ROLE 0x20

#define TRY_SNK_SRC_DISABLE 0xF3

#define PD_POWER_FACTOR     500     /* in mW */
#define PD_VOLT_FACTOR      100     /* in mV */

#define PD_VOLTAGE_5V       5000    /* in mV */
#define PD_VOLTAGE_9V       9000
#define PD_VOLTAGE_12V      12000
#define PD_VOLTAGE_15V      15000
#define PD_VOLTAGE_20V      20000
#define PD_POWER_5W         5000    /* in mW */
#define PD_POWER_7P5W       7500
#define PD_POWER_15W        15000
#define PD_POWER_45W        45000

#define PD_MAX_VOLTAGE_20V  20000
#define PD_MAX_VOLTAGE_21V  21000

#define PD_CURRENT_100MA    100
#define PD_CURRENT_900MA    900
#define PD_CURRENT_1500MA   1500
#define PD_CURRENT_3A       3000


//constant value definition
#define TRANSMIT_BUFFER_SIZE 32
#define RETRY_COUNT 3
#define DELAY_TIME 1000 //us
#define MSG_HEADER_INDEX 0
#define MSG_TYPE_INDEX 1
#define MSG_DATA_INDEX 2
#define MSG_CHECKSUM_LEN 1
#define MSG_TYPE_LEN 1
#define MSG_HEADER_LEN 1
#define MSG_DATA_OBJECT_SIZE 4
#define HIGH_4_BITS_MASK 0x0F
#define SHIFT_4_BITS 4

typedef enum
{
    VALUE_OPEN = 0,
    VALUE_RD = 1,
    VALUE_RA = 2,
    VALUE_USBDEFAULT = 4,
    VALUE_1500MA = 8,
    VALUE_3000MA = 0x0C
}ANX74XX_CC_VALUE;

typedef enum
{
    TYPE_PWR_SRC_CAP = 0x00,
    TYPE_PWR_SNK_CAP = 0x01,
    TYPE_DP_SNK_IDENTITY = 0x02,
    TYPE_SVID = 0x03,
    TYPE_DP_CAP = 0x08,
    TYPE_MAX
}ANX74XX_MSG_TYPE;

//Discovery Identity Command Response definition
#define IDENTITY_OBJECT_COUNT 4
typedef enum
{
    IDENTITY_ID_HEADER_INDEX,
    IDENTITY_CERT_STAT_VDO_INDEX,
    IDENTITY_PRODUCT_VDO_INDEX,
    IDENTITY_AMA_VDO_INDEX
}ANX74XX_IDENTITY_OBJ_INDEX;

#define USB_VENDOR_ID 0x0000
#define AS_USB_DEVICE 1
#define AS_USB_HOST 1
#define NOT_USB_DEVICE 0
#define NOT_USB_HOST 0
#define AMA_MODE 5
#define MODAL_SUPPORTED 1
#define USB_IF_COMPLIANCE 0x000000
#define BCD_DEVICE 0x0000
#define USB_PRODUCT_ID 0x0000
#define HW_VERSION 0x0A
#define FW_VERSION 0x01
#define TX1_CONFIGURABLE 1
#define TX2_CONFIGURABLE 1
#define RX1_CONFIGURABLE 1
#define RX2_CONFIGURABLE 1
#define TX1_FIXED 0
#define TX2_FIXED 0
#define RX1_FIXED 0
#define RX2_FIXED 0
typedef enum
{
    VCONN_POWER_1W,
    VCONN_POWER_15W,
    VCONN_POWER_2W,
    VCONN_POWER_3W,
    VCONN_POWER_4W,
    VCONN_POWER_5W,
    VCONN_POWER_6W
}ANX74XX_VCONN_POWER;

#define VCONN_REQUIRED 1
#define VBUS_REQUIRED 1
#define VCONN_NOT_REQUIRED 0
#define VBUS_NOT_REQUIRED 0
typedef enum
{
    USB_SIGNAL_20,
    USB_SIGNAL_GEN1_20,
    USB_SIGNAL_GEN1_GEN2_20,
    USB_SIGNAL_BILLBOARD
}ANX74XX_USB_SIGNAL_TYPE;

//ID Header
#define ID_HEADER(vid, modal, pt, dev, host) \
(vid | (modal << 26) | (pt << 27) | (dev << 30) | (host << 31))
#define CERT_STAT_VDO(ui) (ui)
#define PRODUCT_VDO(bcd, pid) (bcd | (pid << 16))
#define ATERNATE_MODE_VDO(hw, fw, tx1, tx2, rx1, rx2, vpower, vconn, vbus, sup) \
(sup | (vbus << 3) | (vconn << 4) | (vpower << 5) | (rx2 << 8) | (rx1 << 9) | \
 (tx2 << 10) | (tx1 << 11) | (fw << 24) | (hw << 28))

//Macros for PDO
#define PDO_TYPE_FIXED ((UINT32)0 << 30)
#define PDO_TYPE_BATTERY ((UINT32)1 << 30)
#define PDO_TYPE_VARIABLE ((UINT32)2 << 30)
#define PDO_TYPE_MASK ((UINT32)3 << 30)
#define PDO_FIXED_DUAL_ROLE ((UINT32)1 << 29)   /* Dual role device */
#define PDO_FIXED_SUSPEND_HIGHER ((UINT32)1 << 28)  /* USB Suspend supported */
#define PDO_FIXED_EXTERNAL ((UINT32)1 << 27)    /* Externally powered */
#define PDO_FIXED_COMM_CAP ((UINT32)1 << 26)    /* USB Communications Capable */
#define PDO_FIXED_DATA_SWAP ((UINT32)1 << 25)   /* Data role swap command */
#define PDO_FIXED_UNCHUNKED_SUPPORTED ((UINT32)1 << 24) /*Unchunked extend message*/
#define PDO_FIXED_PEAK_CURR ((UINT32)1 << 20)   /* [21..20] Peak current */
/* Voltage in 50mV units */
#define PDO_FIXED_VOLT(mv) (UINT32)((((UINT32)mv)/50) << 10)
/* Max current in 10mA units */
#define PDO_FIXED_CURR(ma) (UINT32)((((UINT32)ma)/10))

/*build a fixed PDO packet*/
#define PDO_FIXED(mv, ma, flags) \
    (PDO_FIXED_VOLT(mv)\
    | PDO_FIXED_CURR(ma)\
    | (flags))

/*Pos in Data Object, the first index number begin from 0 */
#define PDO_INDEX(n, dat) (dat << (n * PD_ONE_DATA_OBJECT_SIZE*sizeof(UINT8)))
#define PDO_VAR_MAX_VOLT(mv) ((((mv) / 50) & 0x3FF) << 20)
#define PDO_VAR_MIN_VOLT(mv) ((((mv) / 50) & 0x3FF) << 10)
#define PDO_VAR_OP_CURR(ma) ((((ma) / 10) & 0x3FF) << 0)

#define PDO_VAR(min_mv, max_mv, op_ma) \
    (PDO_VAR_MIN_VOLT(min_mv) | PDO_VAR_MAX_VOLT(max_mv) | \
    PDO_VAR_OP_CURR(op_ma) | PDO_TYPE_VARIABLE)
#define PDO_BATT_MAX_VOLT(mv) ((((mv) / 50) & 0x3FF) << 20)
#define PDO_BATT_MIN_VOLT(mv) ((((mv) / 50) & 0x3FF) << 10)
#define PDO_BATT_OP_POWER(mw) ((((mw) / 250) & 0x3FF) << 0)
#define PDO_BATT(min_mv, max_mv, op_mw) \
    (PDO_BATT_MIN_VOLT(min_mv)\
    | PDO_BATT_MAX_VOLT(max_mv)\
    | PDO_BATT_OP_POWER(op_mw)\
    | PDO_TYPE_BATTERY)

#endif  /* __ANX74XXLIBREG_H__ */
