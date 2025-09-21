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

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/04/18   pbitra  Added back full functionality after License update.
11/16/17   mr      Enabled PD Charging
11/06/17   mr      Fix for ANX CC detection issue
07/31/17   mr      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Library Dependencies
*/

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibAnx74xx.h"


/*===========================================================================*/
/*                  FUNCTIONS DEFINITIONS                                    */
/*===========================================================================*/

VOID delay_us(UINT32 us)
{
    gBS->Stall((uint32)us); /* Delay in uSec */
}


EFI_STATUS
EFIAPI
anx74xx_set_fw_feature_control(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_FW_FEATURE_FLAG_BITMAP_TYPE feature_ctrl_bitmap,
    BOOLEAN enable
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UsbPwrCtrl_Device AnxTcpcDev = {0};
    UINT8 data = 0, mask = 0;

    if(feature_ctrl_bitmap >= ANX74XX_FW_FEATURE_CTRL_BITMAP_MAX)
    {
        return EFI_INVALID_PARAMETER;
    }

    CopyMemS(&AnxTcpcDev, sizeof(UsbPwrCtrl_Device), AnxDev, sizeof(UsbPwrCtrl_Device));
    AnxTcpcDev.I2CSlaveCfg.slave_address = ANX74XX_TCPC_ADDR;

    mask = feature_ctrl_bitmap;
    data = (TRUE == enable) ? feature_ctrl_bitmap : 0;

    i2c_sts = anx74xx_write_byte_mask(&AnxTcpcDev, FIRMWARE_CTRL, mask, data, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_ocm_control(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_OCM_CTRL ocm_ctrl,
    BOOLEAN enable
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 data = 0, mask = 0;

    if(ocm_ctrl >= ANX74XX_OCM_CTRL_MAX)
    {
        return EFI_INVALID_PARAMETER;
    }

    mask = ocm_ctrl;
    data = (TRUE == enable) ? ocm_ctrl : 0;

    i2c_sts = anx74xx_write_byte_mask(AnxDev, OCM_CTRL_0, mask, data, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_power_negotiation_parameter(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 max_voltage_mv,
    UINT32 max_power_mw,
    UINT32 min_power_mw
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 max_volt  = max_voltage_mv/PD_VOLT_FACTOR;
    UINT8 max_power = max_power_mw/PD_POWER_FACTOR;
    UINT8 min_power = min_power_mw/PD_POWER_FACTOR;

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, CONFIG_RDO_MAX_VOLTAGE_OFFSET, &max_volt, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, CONFIG_RDO_MAX_POWER_OFFSET, &max_power, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, CONFIG_RDO_MIN_POWER_OFFSET, &min_power, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_rdo_info(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *max_voltage_mv,
    UINT32 *max_power_mw
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 max_volt = 0;
    UINT8 max_power = 0;

    if(!max_voltage_mv || !max_power_mw)  {return EFI_INVALID_PARAMETER;}

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, RDO_MAX_VOLT_OFFSET, &max_volt, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, RDO_MAX_POWER_OFFSET, &max_power, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    *max_voltage_mv = max_volt * PD_VOLT_FACTOR;
    *max_power_mw = max_power * PD_POWER_FACTOR;

    return EFI_SUCCESS;
}

VOID memory_copy(UINT8* des, UINT8* src, UINT8 len)
{
    for(UINT8 i = 0; i < len; i++)
    {
        des[i] = src[i];
    }
}

UINT8 cac_checksum(UINT8 *data, UINT8 len)
{
    UINT8 i;
    UINT8 checksum;

    checksum = 0;
    for (i = 0; i < len; i++)
        checksum += *(data + i);

    return (UINT8) (0 - checksum);
}

EFI_STATUS
EFIAPI
send_pd_msg(
    UsbPwrCtrl_Device *AnxDev,
    UINT8* data,
    UINT8 len,
    UINT8 type)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 byte_cnt = 0, sending_len = 0;
    UINT8 write_data_buf[TRANSMIT_BUFFER_SIZE] = {0};
    BOOLEAN retry_count = TRUE;

    /* full, return 0 */
    write_data_buf[MSG_HEADER_INDEX] = (len + MSG_TYPE_LEN);    /* cmd */
    write_data_buf[MSG_TYPE_INDEX] = type;
    if (data)
        memory_copy(write_data_buf + MSG_DATA_INDEX, data, len);
    /* cmd + checksum */
    write_data_buf[len + MSG_TYPE_LEN + MSG_HEADER_LEN] = cac_checksum(write_data_buf, len + MSG_TYPE_LEN + MSG_HEADER_LEN);

    sending_len = write_data_buf[MSG_HEADER_INDEX] + MSG_HEADER_LEN + MSG_CHECKSUM_LEN;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, TX_BUFFER_OFFSET, &byte_cnt, ANX_DATA_SIZE);

    /* retry */
    if (retry_count && byte_cnt) {
        UINT8 count = RETRY_COUNT;

        while (count) {
            delay_us(DELAY_TIME);
            i2c_sts |= UsbPwrCtrl_i2c_read(AnxDev, TX_BUFFER_OFFSET, &byte_cnt, ANX_DATA_SIZE);
            if (byte_cnt == 0)
                break;
            count--;
        }
    }

    if(I2C_SUCCESS != i2c_sts)
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Read TX_BUFFER Failed __%d \r\n", __FUNCTION__, __LINE__));
        return EFI_DEVICE_ERROR;
    }

    if (byte_cnt == 0) {
        i2c_sts  = UsbPwrCtrl_i2c_write(AnxDev, TX_BUFFER_OFFSET + MSG_HEADER_LEN,
            &write_data_buf[MSG_TYPE_INDEX], sending_len - MSG_HEADER_LEN);
        i2c_sts |= UsbPwrCtrl_i2c_write(AnxDev, TX_BUFFER_OFFSET,
            &write_data_buf[MSG_HEADER_INDEX], MSG_HEADER_LEN);

        if(I2C_SUCCESS != i2c_sts)
        {
            USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Write TX_BUFFER Failed __%d \r\n", __FUNCTION__, __LINE__));
            return EFI_DEVICE_ERROR;
        }
    }

    return 0;
}

EFI_STATUS
EFIAPI
anx74xx_set_source_cap(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *p_src_cap,
    UINT32 no_of_src_cap
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;

    if (p_src_cap)
    {
        i2c_sts = send_pd_msg(AnxDev, (UINT8 *)p_src_cap, no_of_src_cap*MSG_DATA_OBJECT_SIZE, TYPE_PWR_SRC_CAP);
        if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}
    }
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_sink_cap(
    UsbPwrCtrl_Device *AnxDev,
    UINT32 *p_snk_cap,
    UINT32 no_of_snk_cap
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;

    if (p_snk_cap)
    {
        i2c_sts = send_pd_msg(AnxDev, (UINT8 *)p_snk_cap, no_of_snk_cap*MSG_DATA_OBJECT_SIZE, TYPE_PWR_SNK_CAP);
        if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}
    }
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_noncompliant_cable_detect_result(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *noncompliant_cable_detected
)
{
    //TBD, cannot support now
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_irq_mask(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_IRQ_BITMAP_TYPE irq_mask_bitmap
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    // 1: mask, 0: unmask, mask related interrupt
    UINT8 mask = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, IRQ_BITS_MASK_OFFSET, &mask, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    mask |= irq_mask_bitmap;
    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, IRQ_BITS_MASK_OFFSET, &mask, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_irq_unmask(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_IRQ_BITMAP_TYPE irq_mask_bitmap
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    // 1: mask, 0: unmask, unmask related interrupt
    UINT8 mask = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, IRQ_BITS_MASK_OFFSET, &mask, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    mask &= (~irq_mask_bitmap);

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, IRQ_BITS_MASK_OFFSET, &mask, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_is_powered_on(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_powered_on
)
{
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT8 ocm_version = 0;

    if(!is_powered_on)  {return EFI_INVALID_PARAMETER;}

    *is_powered_on = FALSE;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, OCM_VERSION, (UINT8 *)&ocm_version, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  return ((I2C_ERROR_ADDR_NACK == i2c_sts) ? EFI_NOT_READY : EFI_DEVICE_ERROR);

    if (ocm_version)
    {
        *is_powered_on = TRUE;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_is_calib_done(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_calib_done
)
{
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT8 efuse_data[4] = {0};

    if(!is_calib_done)  {return EFI_INVALID_PARAMETER;}

    *is_calib_done = FALSE;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, EFUSE_READ_DATA_0, (UINT8 *)&efuse_data, sizeof(efuse_data));
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    if (!efuse_data[0] || !efuse_data[1] || !efuse_data[2])
        *is_calib_done = FALSE;
    else
        *is_calib_done = TRUE;

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_irq_status(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *irq_status
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 irq = 0;

    if(!irq_status)  {return EFI_INVALID_PARAMETER;}

    *irq_status = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, IRQ_STATUS_OFFSET, (UINT8 *)irq_status, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return ((I2C_ERROR_ADDR_NACK == i2c_sts) ? EFI_NOT_READY : EFI_DEVICE_ERROR);}

    // clear the register
    if (*irq_status)
    {
        i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, IRQ_STATUS_OFFSET, &irq, ANX_DATA_SIZE);
        if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_alert_status(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *alert_0_status,
    UINT8 *alert_1_status
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UsbPwrCtrl_Device AnxTcpcDev = {0};
    UINT16 alert = 0;
    UINT8 clear_int = 0;

    if(!alert_0_status || !alert_1_status)  {return EFI_INVALID_PARAMETER;}

    *alert_0_status = 0;
    *alert_1_status = 0;

    CopyMemS(&AnxTcpcDev, sizeof(UsbPwrCtrl_Device), AnxDev, sizeof(UsbPwrCtrl_Device));
    AnxTcpcDev.I2CSlaveCfg.slave_address = ANX74XX_TCPC_ADDR;

    i2c_sts = UsbPwrCtrl_i2c_read(&AnxTcpcDev, INTR_ALERT_0, (UINT8 *)&alert, sizeof(alert));
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, IRQ_STATUS_OFFSET, (UINT8 *)&clear_int, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    if (alert)
    {
        *alert_0_status = alert & 0xFF;
        *alert_1_status = (alert >> 8) & 0xFF;

        // clear the register
        alert = 0xFFFF;
        i2c_sts = UsbPwrCtrl_i2c_write(&AnxTcpcDev, INTR_ALERT_0, (UINT8 *)&alert, sizeof(alert));
        if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_cc_status(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_CC_STATUS *cc_status
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 status = 0;
    UINT8 cc1 = 0;
    UINT8 cc2 = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, CC_STATUS_OFFSET, &status, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    cc1 = status & HIGH_4_BITS_MASK;
    cc2 = ((status >> SHIFT_4_BITS) & HIGH_4_BITS_MASK);
    if((cc1 == VALUE_OPEN) && (cc2 == VALUE_OPEN))
    {
        cc_status->CC1 = ANX74XX_CCX_STATUS_SRC_OPEN;
        cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_OPEN;
    }
    else if (cc1 == VALUE_OPEN)
    {
        //cc1 is src.open
        cc_status->CC1 = ANX74XX_CCX_STATUS_SRC_OPEN;
        if (cc2 == VALUE_RD)
        {
            //cc2 is src.rd, sink attached, UFP
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RD;
        }
        else if (cc2 == VALUE_RA)
        {
            //cc2 is src.ra, powered cable without sink
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RA;
        }
        else if (cc2 == VALUE_3000MA)
        {
            cc_status->CC2 = ANX74XX_CCX_STATUS_SNK_POWER3P0;
        }
        else if (cc2 == VALUE_1500MA)
        {
            cc_status->CC2 = ANX74XX_CCX_STATUS_SNK_POWER1P5;
        }
        else
        {
            if (cc2 == VALUE_USBDEFAULT)
                cc_status->CC2 = ANX74XX_CCX_STATUS_SNK_DEFAULT;
        }
    }
    else if (cc1 == VALUE_RD)
    {
        //cc1 is src.rd
        cc_status->CC1 = ANX74XX_CCX_STATUS_SRC_RD;
        if (cc2 == VALUE_OPEN)
        {
            // ufp
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_OPEN;
        }
        else if (cc2 == VALUE_RD)
        {
            //src.rd
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RD;
        }
        else if (cc2 == VALUE_RA)
        {
            //src.ra
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RA;
        }
    }
    else if (cc1 == VALUE_RA)
    {
        //cc1 is src.ra
        cc_status->CC1 = ANX74XX_CCX_STATUS_SRC_RA;
        if (cc2 == VALUE_OPEN)
        {
            //powered able without ufp
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_OPEN;
        }
        else if (cc2 == VALUE_RD)
        {
            //src.rd, powered cable with ufp
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RD;
        }
        else if (cc2 == VALUE_RA)
        {
            //src.ra, audio accessory
            cc_status->CC2 = ANX74XX_CCX_STATUS_SRC_RA;
        }
    }
    else
    {
        if ((cc1 == VALUE_USBDEFAULT))
        {
            //usbdefault
            cc_status->CC1 = ANX74XX_CCX_STATUS_SNK_DEFAULT;
        }
        else if ((cc1 == VALUE_1500MA))
        {
            //1500ma
            cc_status->CC1 = ANX74XX_CCX_STATUS_SNK_POWER1P5;
        }
        else if ((cc1 == VALUE_3000MA))
        {
            //3000ma
            cc_status->CC1 = ANX74XX_CCX_STATUS_SNK_POWER3P0;
        }
    }


    return EFI_SUCCESS;

}

EFI_STATUS
EFIAPI
anx74xx_get_misc_status(
    UsbPwrCtrl_Device *AnxDev,
    BOOLEAN *is_dfp,
    BOOLEAN *is_power_provider,
    BOOLEAN *is_vconn_on
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 status = 0;

    if(!is_dfp || !is_power_provider || !is_vconn_on)  {return EFI_INVALID_PARAMETER;}

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, SYSTEM_STATUS_OFFSET, &status, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return ((I2C_ERROR_ADDR_NACK == i2c_sts) ? EFI_NOT_READY : EFI_DEVICE_ERROR);}

    *is_dfp = (status & DATA_ROLE) ? TRUE : FALSE;
    *is_power_provider = (status & VBUS_STATUS) ? TRUE : FALSE;
    *is_vconn_on = (status & VCONN_STATUS) ? TRUE : FALSE;

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_port_role(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_PORT_ROLE_TYPE  port_role
)
{
    //Set port role need send Identity message when initializing
    UINT32 identity[IDENTITY_OBJECT_COUNT] = {0};
    // UINT8 data_role = 1;
    switch(port_role)
    {
        case ANX74XX_PORT_ROLE_DRP:
        identity[IDENTITY_ID_HEADER_INDEX] = ID_HEADER(USB_VENDOR_ID,
                    MODAL_SUPPORTED, AMA_MODE, AS_USB_DEVICE, AS_USB_HOST);
        break;
        case ANX74XX_PORT_ROLE_DFP_ONLY:
        identity[IDENTITY_ID_HEADER_INDEX] = ID_HEADER(USB_VENDOR_ID,
                    MODAL_SUPPORTED, AMA_MODE, NOT_USB_DEVICE, AS_USB_HOST);
        break;
        case ANX74XX_PORT_ROLE_UFP_ONLY:
        identity[IDENTITY_ID_HEADER_INDEX] = ID_HEADER(USB_VENDOR_ID,
                    MODAL_SUPPORTED, AMA_MODE, AS_USB_DEVICE, NOT_USB_HOST);
        break;
        default:
        return EFI_DEVICE_ERROR;
    }
    identity[IDENTITY_CERT_STAT_VDO_INDEX] = CERT_STAT_VDO(USB_IF_COMPLIANCE);
    identity[IDENTITY_PRODUCT_VDO_INDEX] = PRODUCT_VDO(BCD_DEVICE,
                                            USB_PRODUCT_ID);
    identity[IDENTITY_AMA_VDO_INDEX] = ATERNATE_MODE_VDO(HW_VERSION,
                            FW_VERSION, TX1_FIXED, TX2_FIXED, RX1_FIXED,
                            RX2_FIXED, VCONN_POWER_15W, VCONN_REQUIRED,
                            VBUS_REQUIRED, USB_SIGNAL_GEN1_20);
    send_pd_msg(AnxDev, (UINT8 *)identity,
                IDENTITY_OBJECT_COUNT*MSG_DATA_OBJECT_SIZE,
                TYPE_DP_SNK_IDENTITY);
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_get_port_power_role(
    UsbPwrCtrl_Device *AnxDev,
    ANX74XX_POWER_ROLE_TYPE* power_role
)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 status = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, SYSTEM_STATUS_OFFSET, &status, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)  {return EFI_DEVICE_ERROR;}

    if (status & VBUS_STATUS)
    {
        *power_role = ANX74XX_POWER_ROLE_SOURCE;
    }
    else
    {
        *power_role = ANX74XX_POWER_ROLE_SINK;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
anx74xx_set_initial_msg_info(
    UsbPwrCtrl_Device *AnxDev,
    UINT8 *p_msg,
    UINT8 msg_size,
    UINT8 message_type
)
{
    if (p_msg)
    {
        send_pd_msg(AnxDev, p_msg, msg_size, message_type);
    }

    return EFI_SUCCESS;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTIONS DEFINITIONS                           */
/*===========================================================================*/
i2c_status anx74xx_write_byte_mask(UsbPwrCtrl_Device *AnxDev, UINT8 offset, UINT8 mask, UINT8 data_in, UINT8 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT8 data = 0;

    i2c_sts = UsbPwrCtrl_i2c_read(AnxDev, offset, (UINT8 *)&data, sizeof(data));
    if(I2C_SUCCESS != i2c_sts) {return i2c_sts;}

    data &= (~mask);
    data |= (data_in & mask);

    i2c_sts = UsbPwrCtrl_i2c_write(AnxDev, offset, (UINT8 *)&data, size_in);

    return i2c_sts;
}
