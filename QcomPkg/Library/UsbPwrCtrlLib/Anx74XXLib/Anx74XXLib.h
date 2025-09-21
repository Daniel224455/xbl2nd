
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

#ifndef __ANX74XXLIB_H__
#define __ANX74XXLIB_H__

#include <Uefi.h>

#include "Anx74XXLibReg.h"

typedef enum
{
    ANX74XX_FW_FEATURE_CTRL_BITMAP_AUTO_PD_EN            = 1 << 1,   //bit 1
    ANX74XX_FW_FEATURE_CTRL_BITMAP_TRY_SRC_ENABLE        = 1 << 2,   //bit 2
    ANX74XX_FW_FEATURE_CTRL_BITMAP_TRY_SNK_ENABLE        = 1 << 3,   //bit 3
    ANX74XX_FW_FEATURE_CTRL_BITMAP_GOTO_SAFE5V_EN        = 1 << 4,   //bit 4
    ANX74XX_FW_FEATURE_CTRL_BITMAP_MAX
}ANX74XX_FW_FEATURE_FLAG_BITMAP_TYPE;

typedef enum
{
    ANX74XX_IRQ_BITMAP_RECVD_ACK                = 1 << 1,   //bit 1
    ANX74XX_IRQ_BITMAP_VCONN_CHG_INT            = 1 << 2,   //bit 2
    ANX74XX_IRQ_BITMAP_VBUS_CHG_INT             = 1 << 3,   //bit 3
    ANX74XX_IRQ_BITMAP_CC_STATUS_CHG_INT        = 1 << 4,   //bit 4
    ANX74XX_IRQ_BITMAP_DATA_ROLE_CHG_INT        = 1 << 5,   //bit 5
    ANX74XX_IRQ_BITMAP_PR_C_GOT_POWER           = 1 << 6    //bit 6
}ANX74XX_IRQ_BITMAP_TYPE;

typedef enum
{
    /* Internal Alert_0 Interrupt */
    ANX74XX_ALERT_0_SOFTWARE_INTR               = 1 << 6,   //bit 6
    ANX74XX_ALERT_0_RECEIVED_MSG_INTR           = 1 << 7,   //bit 7
}ANX74XX_ALERT_0_BITMAP_TYPE;

typedef enum
{
    /* Internal Alert_1 Interrupt */
    ANX74XX_ALERT_1_INTP_POWER_OFF_INTR         = 1 << 6,   //bit 6
    ANX74XX_ALERT_1_INTP_POWER_ON_INTR          = 1 << 7    //bit 7
}ANX74XX_ALERT_1_BITMAP_TYPE;

typedef enum
{
    ANX74XX_CCX_STATUS_SRC_OPEN                 = 0x0,
    ANX74XX_CCX_STATUS_SRC_RD                   = 0x1,
    ANX74XX_CCX_STATUS_SRC_RA                   = 0x2,
    ANX74XX_CCX_STATUS_SNK_DEFAULT              = 0x4,
    ANX74XX_CCX_STATUS_SNK_POWER1P5             = 0x8,
    ANX74XX_CCX_STATUS_SNK_POWER3P0             = 0xC
}ANX74XX_CCX_STATUS_TYPE;

/* ANX74XX_PORT_DETECT_<SRC/SNK>_<CC1>_<CC2> */
typedef enum
{
    ANX74XX_PORT_DETECT_SRC_NONE                = 0b00000000,
    /* SRC_RD_OPEN */
    ANX74XX_PORT_DETECT_SRC_RD_X                = 0b00000001,
    ANX74XX_PORT_DETECT_SRC_X_RD                = 0b00010000,
    /* SRC_UNORIENTED_RD_RD - Debug Accesory */
    ANX74XX_PORT_DETECT_SRC_RD_RD               = 0b00010001,
    /* SRC_RA_OPEN */
    ANX74XX_PORT_DETECT_SRC_RA_X                = 0b00000010,
    ANX74XX_PORT_DETECT_SRC_X_RA                = 0b00100000,
    /* SRC_RA_RA */
    ANX74XX_PORT_DETECT_SRC_RA_RA               = 0b00100010,
    /* SRC_RD_RA_VCONN */
    ANX74XX_PORT_DETECT_SRC_RD_RA               = 0b00100001,
    ANX74XX_PORT_DETECT_SRC_RA_RD               = 0b00010010,
    /* SNK_DEFAULT */
    ANX74XX_PORT_DETECT_SNK_DFLT_X              = 0b00000100,
    ANX74XX_PORT_DETECT_SNK_X_DFLT              = 0b01000000,
    /* SNK_DEBUG - Debug Accesory */
    ANX74XX_PORT_DETECT_SNK_DFLT_DFLT           = 0b01000100,
    /* SNK_1P5 */
    ANX74XX_PORT_DETECT_SNK_1P5_X               = 0b00001000,
    ANX74XX_PORT_DETECT_SNK_X_1P5               = 0b10000000,
    /* SNK_DEBUG - Debug Accesory */
    ANX74XX_PORT_DETECT_SNK_1P5_1P5             = 0b10001000,
    /* SNK_3P0 */
    ANX74XX_PORT_DETECT_SNK_3P0_X               = 0b00001100,
    ANX74XX_PORT_DETECT_SNK_X_3P0               = 0b11000000,
    /* SNK_DEBUG - Debug Accesory */
    ANX74XX_PORT_DETECT_SNK_3P0_3P0             = 0b11001100
}ANX74XX_PORT_DETECT_TYPE;

typedef enum
{
    ANX74XX_OCM_CTRL_OCM_RESET                  = 1 << 6,   //bit 6
    ANX74XX_OCM_CTRL_OCM_INT_GATE               = 1 << 7,   //bit 7
    ANX74XX_OCM_CTRL_MAX
}ANX74XX_OCM_CTRL;

typedef enum
{
    ANX74XX_PORT_ROLE_DRP,
    ANX74XX_PORT_ROLE_DFP_ONLY,
    ANX74XX_PORT_ROLE_UFP_ONLY,
}ANX74XX_PORT_ROLE_TYPE;

typedef struct
{
    ANX74XX_CCX_STATUS_TYPE     CC1;
    ANX74XX_CCX_STATUS_TYPE     CC2;
}ANX74XX_CC_STATUS;

typedef enum
{
    ANX74XX_POWER_ROLE_SINK,
    ANX74XX_POWER_ROLE_SOURCE
}ANX74XX_POWER_ROLE_TYPE;


/*
    APIs to write masked data on ANX41XX Device
    @param slave_addr  - I2C Slave Address of the HW
    @param offset      - Register Offset Address
    @param mask        - Mask for the Data to be written
    @param data_in     - Data to be written
    @param size_in     - Size of data to be written

    @return Returns 'Count' of successful read/written byte(s) or 0
 */
i2c_status anx74xx_write_byte_mask(UsbPwrCtrl_Device *AnxDev, UINT8 offset, UINT8 mask, UINT8 data_in, UINT8 size_in);

/*
    API to enable disable firmware feature
    @param slave_addr - I2C Slave Address of the HW
    @param feature_ctrl_bitmap - bitmap to enable/disable the feature
    @param enable - TRUE/FALSE to enable/disable the feature

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_fw_feature_control(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_FW_FEATURE_FLAG_BITMAP_TYPE feature_ctrl_bitmap,
    BOOLEAN enable
);

/*
    API to enable disable firmware feature
    @param slave_addr - I2C Slave Address of the HW
    @param ocm_ctrl - bitmap to enable/disable OCM control
    @param enable - TRUE/FALSE to enable/disable the feature

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_ocm_control(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_OCM_CTRL ocm_ctrl,
    BOOLEAN enable
);

/*
    API to configure parameters for power negotiation
    @param slave_addr - I2C Slave Address of the HW
    @param max_voltage_mv - max voltage in unit of mili-volt
    @param max_power_mw - max power in unit of mili-watt
    @param min_power_mw - min power in unit of mili-watt

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_power_negotiation_parameter(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 max_voltage_mv,
    UINT32 max_power_mw,
    UINT32 min_power_mw
);

/*
    API to read RDO from HW
    @param slave_addr - I2C Slave Address of the HW
    @param max_voltage_mv - max voltage in unit of mili-volt
    @param max_power_mw - max power in unit of mili-watt

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_rdo_info(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *max_voltage_mv,
    UINT32 *max_power_mw
);

/*
    API to set Source capability PDO
    @param slave_addr - I2C Slave Address of the HW
    @param p_src_cap - pointer to a array of source PDOs
    @param no_of_src_cap - number of source cap are passed in the previous argument

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_source_cap(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *p_src_cap,
    UINT32 no_of_src_cap
);

/*
    API to set Sink capability PDO
    @param slave_addr - I2C Slave Address of the HW
    @param p_snk_cap - pointer to a array of Sink PDOs
    @param no_of_snk_cap - number of sink cap are passed in the previous argument

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_sink_cap(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *p_snk_cap,
    UINT32 no_of_snk_cap
);

/*
    API to set initial PD messages information (Identity Response, SVID response, DP capabilities)
    @param slave_addr - I2C Slave Address of the HW
    @param p_msg - pointer to a array of PD message content
    @param msg_size - bytes count of object are passed in the previous argument
    @param message_type - type of PD message to initialize, please refer the definition
                          ANX74XX_MSG_TYPE in Anx74XXLibReg.h
                          typedef enum
                          {
                            TYPE_PWR_SRC_CAP = 0x00,
                            TYPE_PWR_SNK_CAP = 0x01,
                            TYPE_DP_SNK_IDENTITY = 0x02,
                            TYPE_SVID = 0x03,
                            TYPE_DP_CAP = 0x08,
                            TYPE_MAX
                          }ANX74XX_MSG_TYPE;

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_initial_msg_info(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *p_msg,
    UINT8 msg_size,
    UINT8 message_type
);

/*
    API to get the value of global variable which indicating
    non-compliant C-to-A cable is detected
    @param slave_addr - I2C Slave Address of the HW
    @param noncompliant_cable_detected - return True if non-compliant cable is detected

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_noncompliant_cable_detect_result(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *noncompliant_cable_detected
);

/*
    API to set irq_mask
    @param slave_addr - I2C Slave Address of the HW
    @param irq_mask_bitmap - bitmap to set the irq mask

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_irq_mask(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_IRQ_BITMAP_TYPE irq_mask_bitmap
);

/*
    API to unmask irq
    @param slave_addr - I2C Slave Address of the HW
    @param irq_mask_bitmap - bitmap to set the irq mask

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_irq_unmask(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_IRQ_BITMAP_TYPE irq_mask_bitmap
);

/*
    API to check if ANX Internal MCU is Power-ed On
    @param slave_addr - I2C Slave Address of the HW
    @param is_powered_on - return power-on status

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_is_powered_on(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_powered_on
);

/*
    API to check if chip calibration is done for PD
    @param slave_addr - I2C Slave Address of the HW
    @param is_calib_done - return calibration status

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_is_calib_done(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_calib_done
);

/*
    API to get irq status
    @param slave_addr - I2C Slave Address of the HW
    @param irq_status_bitmap - return irq status

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_irq_status(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *irq_status_bitmap
);

/*
    API to get irq alert status
    @param slave_addr - I2C Slave Address of the HW
    @param alert_0/1_status - return irq alert status

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_alert_status(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *alert_0_status,
    UINT8 *alert_1_status
);

/*
    API to get CC status
    @param slave_addr - I2C Slave Address of the HW
    @param cc_status - return cc status

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_cc_status(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_CC_STATUS *cc_status
);

/*
    API to get misc status, including Data role, Vbus Status and Vconn Status
    @param slave_addr - I2C Slave Address of the HW
    @param is_dfp - return TRUE if current data role is UFP
    @param is_power_provider - return TRUE if the port is provinding power
    @param is_vconn_on - return TRUE if the vconn is On

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_get_misc_status(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_dfp,
    BOOLEAN *is_power_provider,
    BOOLEAN *is_vconn_on
);

/*
    API to set port role to DRP, UFP oply or DFP only.
    @param slave_addr - I2C Slave Address of the HW
    @param port_role - port role to be set

    @return EFI_SUCCESS if the operation is successful.
            Other Error code if error has been detected
 */
EFI_STATUS
EFIAPI
anx74xx_set_port_role(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_PORT_ROLE_TYPE  port_role
);

#endif  /* __ANX74XXLIB_H__ */
