/**
@file  UsbPwrCtrlLibAnx74xx.c
@brief Implementation of UsbPWrCtrlLibrary API for ANX TypeC Controller module
*/
/*=============================================================================
Copyright (c) 2017-2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/26/18   mr      Revert ANX Data role fix change. 'CustomFlash' for ANX chip
                    fixes PD and Data role related issues in UEFI.
04/17/18   mr      ANX Lib update for Thanos ER DATA Role issue
03/15/18   mr      Fix for Thanos ER device PD negotiation issue
03/02/18   mr      Dynamic TypeC Role control for USB hotplug cases
02/01/18   mr      Implemented low level ANX Role select API
01/29/18   dc      Making sure Auto PD is enabled after Policy is updated
01/18/18   mr      Fix for XBL Crash issue in TypeC/PD Status APIs
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
#include "string.h"

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibAnx74xx.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
typedef enum _USB_PWR_CTRL_ANX74XX_POWER_STATE
{
    USB_PWR_CTRL_ANX74XX_POWER_STATE_INVALID,
    USB_PWR_CTRL_ANX74XX_POWER_STATE_NONE,      /* No change from last power state, CC state may change */
    USB_PWR_CTRL_ANX74XX_POWER_STATE_OFF,       /* ANX Internal MCU is Power-ed Off state */
    USB_PWR_CTRL_ANX74XX_POWER_ON_EVENT,        /* ANX Power-On Interrupt received */
    USB_PWR_CTRL_ANX74XX_POWER_OFF_EVENT,       /* ANX Power-Off Interrupt received */
    USB_PWR_CTRL_ANX74XX_NON_POWER_EVENT,       /* ANX Non-Power Interrupt received */
    USB_PWR_CTRL_ANX74XX_POWER_STATE_MAX
} USB_PWR_CTRL_ANX74XX_POWER_STATE;


/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS Anx74xx_PreInit(EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *TypeCExtDep);
EFI_STATUS Anx74xx_Handle_PowerState(UINT8 *PowerState);
VOID EFIAPI Anx74xx_StatusChngCb(IN EFI_EVENT Event, IN VOID *Context);


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
UsbPwrCtrl_Device gAnx74xxDevice = {0};

static EFI_EVENT TypeCStsEvt = NULL;
static EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS gTypeCPortStatus = {0};
static EFI_USB_PWR_CTRL_PD_STATUS gPDStatus = {0};
static EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE gTypeCPortRole = 0;

/**
  Initialize the interface to ANX TypeC Controller module

  @param TypeC                  Refer EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_Init(EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO TypeC)
{
    #define TYPEC_STATUS_POLL_INTERVAL      (10 * 10000U)  /* 10 mSec */

    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT8 data = 0;

    Status = Anx74xx_PreInit(TypeC.HwExtDep);
    if(EFI_ERROR(Status)) {return Status;}

    gAnx74xxDevice.hI2CHandle = NULL;
    gAnx74xxDevice.I2CInstance = TypeC.HwInfo.ResourceIndex;

    gAnx74xxDevice.I2CSlaveCfg.bus_frequency_khz = 400;  /* 400 KHz */
    gAnx74xxDevice.I2CSlaveCfg.slave_address = TypeC.HwInfo.ResourceIDorAddr;
    gAnx74xxDevice.I2CSlaveCfg.mode = I2C;
    gAnx74xxDevice.I2CSlaveCfg.slave_max_clock_stretch_us = 0;
    gAnx74xxDevice.I2CSlaveCfg.core_configuration1 = 0;
    gAnx74xxDevice.I2CSlaveCfg.core_configuration2 = 0;

    gAnx74xxDevice.I2CSlaveRegMap = ANX_REG_ADDR_SIZE;  /* Register Address width is 1Byte/8Bits */
    gAnx74xxDevice.TimeoutMs = 0;  /* Need to revisit */

    i2c_sts = i2c_open(gAnx74xxDevice.I2CInstance, &gAnx74xxDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts || NULL == gAnx74xxDevice.hI2CHandle)
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a i2c_open Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    DELAY_uS(1000);  /* 1 msec wait - required before 1st ANX Register read/write */

    /* Set Auto_PD and related Config */
    UsbPwrCtrl_Device Anx74xxTcpcDevice = gAnx74xxDevice;
    Anx74xxTcpcDevice.I2CSlaveCfg.slave_address = ANX74XX_TCPC_ADDR;
    data = 0x52;  /* enable_auto_pd | enable_safe_5v | force_send_rdo */
    i2c_sts = UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, FIRMWARE_CTRL, (UINT8 *)&data, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts || NULL == gAnx74xxDevice.hI2CHandle)
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a i2c_write Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    /* Create a timer to poll on TypeC Status change */
    Status = gBS->CreateEvent(
                EVT_TIMER | EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                Anx74xx_StatusChngCb,
                NULL,
                &TypeCStsEvt
                );
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a TypeC Status change CreateEvent Failed \r\n", __FUNCTION__));
        return Status;
    }

    gBS->SignalEvent(TypeCStsEvt);

    /* Set the polling period for the USB TypeC periodic event */
    Status = gBS->SetTimer(
                TypeCStsEvt,
                TimerPeriodic,
                TYPEC_STATUS_POLL_INTERVAL
                );
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a TypeC Status event SetTimer Failed \r\n", __FUNCTION__));
        return Status;
    }

    USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Anx74xx_Init Success \r\n", __FUNCTION__));

    return Status;
}

/**
  De-initialize the interface to ANX TypeC Controller module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_Deinit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;

    i2c_sts = i2c_close(gAnx74xxDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
  Get TypeC Port Status

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_GetTypeCPortStatus(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus)
{
    EFI_TPL OldTpl;

    // Sanity Check
    if (TypeCPortStatus == NULL) {return EFI_INVALID_PARAMETER;}

    /* Enter into critical section to avoid multiple client calls at the same time */
    OldTpl = gBS->RaiseTPL(TPL_CALLBACK);

    CopyMemS(TypeCPortStatus, sizeof(EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS),
                &gTypeCPortStatus, sizeof(EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS));

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}

/**
  Get TypeC PD Status

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_GetPDStatus(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus)
{
    EFI_TPL OldTpl;

    // Sanity Check
    if (PDStatus == NULL) {return EFI_INVALID_PARAMETER;}

    /* Enter into critical section to avoid multiple client calls at the same time */
    OldTpl = gBS->RaiseTPL(TPL_CALLBACK);

    CopyMemS(PDStatus, sizeof(EFI_USB_PWR_CTRL_PD_STATUS),
                &gPDStatus, sizeof(EFI_USB_PWR_CTRL_PD_STATUS));

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}

/**
  Enable/Disable Vconn

  @param[IN]    HwInfo          Specify which port the request is for
  @param[IN]    bVconnEn        Enable/Dsiable Vconn

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_SetVconnEn(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN bVconnEn)
{
    /* Vconn is auto controlled by ANX H/w */
    return EFI_UNSUPPORTED;
}

/**
  Set TypeC Port Role

  @param[IN]    HwInfo          Specify H/w info
  @param[IN]    TypeCPortRole   TypeC Port Role

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_SetTypeCPortRole(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE TypeCPortRole)
{
    EFI_STATUS Status = EFI_SUCCESS;
    ANX74XX_PORT_ROLE_TYPE Anx74xxPortRole;

    // USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a TypeCPortRole: %d \r\n", __FUNCTION__, TypeCPortRole));

    gTypeCPortRole = TypeCPortRole;

    switch(TypeCPortRole)
    {
      case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK:
        Anx74xxPortRole = ANX74XX_PORT_ROLE_UFP_ONLY;
        break;
      case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SOURCE:
        Anx74xxPortRole = ANX74XX_PORT_ROLE_DFP_ONLY;
        break;
      case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_DUAL:
        Anx74xxPortRole = ANX74XX_PORT_ROLE_DRP;
        break;
      default:
        gTypeCPortRole = EFI_USB_PWR_CTRL_TYPEC_POWERROLE_UNKNOWN;
        return EFI_INVALID_PARAMETER;
    }

    /* Set Port Role */
    Status = anx74xx_set_port_role(&gAnx74xxDevice, Anx74xxPortRole);
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Set Port Role Failed \r\n", __FUNCTION__));
    }

    return Status;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTION DEFINITIONS                            */
/*===========================================================================*/
EFI_STATUS
Anx74xx_PreInit(EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *TypeCExtDep)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;
    UINT8 NumDep = 0;

    if(NULL == TypeCExtDep) {return EFI_INVALID_PARAMETER;}

    Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void**)&TLMMProtocol);
    if(EFI_ERROR(Status) || NULL == TLMMProtocol)
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    for(NumDep = 0; NumDep < USB_PWR_CTRL_ANX74XX_EXT_DEP_MAX; NumDep++)
    {
        if( (TypeCExtDep[NumDep].InitState == EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE_HIZ) ||
            (TypeCExtDep[NumDep].InitState >= EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE_INVALID) )
            continue;

        switch(TypeCExtDep[NumDep].ExtDepResource)
        {
          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE:
            {
              Status  = TLMMProtocol->ConfigGpio(
                          (UINT32)EFI_GPIO_CFG(TypeCExtDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          TLMM_GPIO_ENABLE);

              Status |= TLMMProtocol->GpioOut(
                          (UINT32)EFI_GPIO_CFG(TypeCExtDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          ((EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH == TypeCExtDep[NumDep].InitState) ? GPIO_HIGH_VALUE : GPIO_LOW_VALUE));

              if(EFI_ERROR(Status))
              {
                  USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a TLMMProtocol APIs Failed, Status [%r] \r\n", __FUNCTION__, Status));
                  return Status;
              }

              USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Resource[%d] Enable Success \r\n", __FUNCTION__, TypeCExtDep[NumDep].ResourceID));
              // DELAY_uS(1000);  /* 1 Msec wait */
            }
            break;

          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_PMIC_RESOURCE:
          default:
            continue;
        }
    }

    return Status;
}

VOID
EFIAPI Anx74xx_StatusChngCb(
  IN EFI_EVENT Event,
  IN VOID *Context)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT8 cc_status = 0, power_state = 0;
    BOOLEAN data_role = 0, is_power_provider = 0, is_vconn_on = 0;
    UINT32 max_voltage_mv = 0, max_power_mw = 0;

    Status = Anx74xx_Handle_PowerState(&power_state);
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a _Handle_PowerState Failed \r\n", __FUNCTION__));
        return;
    }

    /* TypeC status remains same. Do not update for _NONE case */
    if(power_state == USB_PWR_CTRL_ANX74XX_POWER_STATE_NONE)
        return;

    memset(&gTypeCPortStatus, 0, sizeof(EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS));
    memset(&gPDStatus, 0, sizeof(EFI_USB_PWR_CTRL_PD_STATUS));

    if( (power_state == USB_PWR_CTRL_ANX74XX_POWER_STATE_OFF) || (power_state == USB_PWR_CTRL_ANX74XX_POWER_OFF_EVENT) )
        return;

    i2c_sts = UsbPwrCtrl_i2c_read(&gAnx74xxDevice, CC_STATUS_OFFSET, &cc_status, ANX_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a CC_STATUS read Failed \r\n", __FUNCTION__));
        return;
    }

    if(ANX74XX_PORT_DETECT_SRC_NONE == cc_status) {return;}

    Status = anx74xx_get_misc_status(&gAnx74xxDevice, &data_role, &is_power_provider, &is_vconn_on);
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a _get_misc_status Failed \r\n", __FUNCTION__));
        return;
    }

    // USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a cc_status: 0x%X, data_role:%d, is_power_provider:%d, is_vconn_on:%d, power_state:%d \r\n",
                            // __FUNCTION__, cc_status, data_role, is_power_provider, is_vconn_on, power_state));

    /* Update TypeC Status */
    gTypeCPortStatus.ConnectSts = TRUE;
    gTypeCPortStatus.PowerRole = (TRUE == is_power_provider) ?
                                    EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SOURCE :
                                    EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK;

    gTypeCPortStatus.DataRole = (TRUE == data_role) ?
                                    EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP :
                                    EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP;

    switch(cc_status)
    {
      case ANX74XX_PORT_DETECT_SRC_RD_X:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_OPEN;
        break;
      case ANX74XX_PORT_DETECT_SRC_X_RD:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_OPEN;
        break;
      case ANX74XX_PORT_DETECT_SRC_RD_RD:
        /* CCOrientation=? */
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_UNORIENTED_RD_RD;
        break;
      case ANX74XX_PORT_DETECT_SRC_RA_X:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RA_OPEN;
        break;
      case ANX74XX_PORT_DETECT_SRC_X_RA:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RA_OPEN;
        break;
      case ANX74XX_PORT_DETECT_SRC_RA_RA:
        /* CCOrientation=? */
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RA_RA;
        break;
      case ANX74XX_PORT_DETECT_SRC_RD_RA:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_RA_VCONN;
        break;
      case ANX74XX_PORT_DETECT_SRC_RA_RD:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSourceType = EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_RA_VCONN;
        break;
      case ANX74XX_PORT_DETECT_SNK_DFLT_X:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEFAULT;
        break;
      case ANX74XX_PORT_DETECT_SNK_X_DFLT:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEFAULT;
        break;
      case ANX74XX_PORT_DETECT_SNK_DFLT_DFLT:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEBUG;
        break;
      case ANX74XX_PORT_DETECT_SNK_1P5_X:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5;
        break;
      case ANX74XX_PORT_DETECT_SNK_X_1P5:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5;
        break;
      case ANX74XX_PORT_DETECT_SNK_1P5_1P5:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEBUG;
        break;
      case ANX74XX_PORT_DETECT_SNK_3P0_X:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0;
        break;
      case ANX74XX_PORT_DETECT_SNK_X_3P0:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0;
        break;
      case ANX74XX_PORT_DETECT_SNK_3P0_3P0:
        gTypeCPortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
        gTypeCPortStatus.DetectedSinkType = EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEBUG;
        break;
      default:
        break;
    }

    /* Update PD Status */
    Status = anx74xx_get_rdo_info(&gAnx74xxDevice, &max_voltage_mv, &max_power_mw);
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a _get_rdo_info Failed \r\n", __FUNCTION__));
        return;
    }

    // USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a max_voltage_mv:%d, max_power_mw:%d \r\n", __FUNCTION__, max_voltage_mv, max_power_mw));

    gPDStatus.bPDConsumer = (FALSE == is_power_provider) ? TRUE : FALSE;
    gPDStatus.PDConsumerState.MaxVoltageMV = max_voltage_mv;
    gPDStatus.PDConsumerState.MaxCurrentMA = (max_power_mw  * 1000) / max_voltage_mv;

    return;
}

EFI_STATUS
Anx74xx_Handle_PowerState(UINT8 *PowerState)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT8 mask = 0, data = 0, is_powered_on = 0, intr_alert_0 = 0, intr_alert_1 = 0;
    UINT8 hw_chip_rev = 0;
    UsbPwrCtrl_Device Anx74xxTcpcDevice = gAnx74xxDevice;

    if(!PowerState) {return EFI_INVALID_PARAMETER;}

    Anx74xxTcpcDevice.I2CSlaveCfg.slave_address = ANX74XX_TCPC_ADDR;

    *PowerState = USB_PWR_CTRL_ANX74XX_POWER_STATE_NONE;

    Status = anx74xx_is_powered_on(&gAnx74xxDevice, &is_powered_on);
    if(FALSE == is_powered_on)
    {
        // USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a ANX is Power-ed Off state \r\n", __FUNCTION__));
        Status = EFI_SUCCESS;
        *PowerState = USB_PWR_CTRL_ANX74XX_POWER_STATE_OFF;
        goto ExitFunction;
    }

    Status = anx74xx_get_alert_status(&gAnx74xxDevice, &intr_alert_0, &intr_alert_1);
    if(EFI_ERROR(Status))
    {
        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Read ALEART Status Failed \r\n", __FUNCTION__));
        goto ExitFunction;
    }

    // if(intr_alert_0 || intr_alert_1)
        // USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a intr_alert_0: 0x%02X, intr_alert_1: 0x%02X \r\n", __FUNCTION__, intr_alert_0, intr_alert_1));

    if(intr_alert_1 & ANX74XX_ALERT_1_INTP_POWER_ON_INTR)
    {
        UINT8 is_calib_done = FALSE;

        Status = anx74xx_is_calib_done(&gAnx74xxDevice, &is_calib_done);
        if(EFI_ERROR(Status))
        {
            USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a _is_calib_done Failed \r\n", __FUNCTION__));
            goto ExitFunction;
        }

        /* Chip may not calibrated, check if needs calibration */
        if (FALSE == is_calib_done)
        {
            i2c_sts = UsbPwrCtrl_i2c_read(&gAnx74xxDevice, HW_CHIP_REV, (UINT8 *)&hw_chip_rev, ANX_DATA_SIZE);

            // USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a hw_chip_rev: 0x%02X, is_calib_done: %d \r\n", __FUNCTION__, hw_chip_rev, is_calib_done));

            if (hw_chip_rev == AC_CHIP_REV)
            {
                /* 27MHz Clock Control */
                data = 0xB7;
                i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, RING_OSC_CTRL, (UINT8 *)&data, ANX_DATA_SIZE);

                /* ADC Ref Select and Adjustment */
                data = 0x26;
                i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, ADC_CTRL_2, (UINT8 *)&data, ANX_DATA_SIZE);
            }
            else
            {
                /* 27MHz Clock Control */
                data = 0xCD;
                i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, RING_OSC_CTRL, (UINT8 *)&data, ANX_DATA_SIZE);

                /* ADC Ref Select and Adjustment */
                data = 0x0f;
                i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, ADC_CTRL_2, (UINT8 *)&data, ANX_DATA_SIZE);
            }

            /* 5.1K Pull-down register Config */
            data = 0x11;
            i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, ANALOG_CTRL_6, (UINT8 *)&data, ANX_DATA_SIZE);
        }

        //configure CC status mask
        Status = anx74xx_set_irq_unmask(&gAnx74xxDevice, ANX74XX_IRQ_BITMAP_CC_STATUS_CHG_INT);

        if(EFI_ERROR(Status) || I2C_SUCCESS != i2c_sts)
        {
            USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a ANX Firmware Power-On Config Failed \r\n", __FUNCTION__));
            Status = EFI_DEVICE_ERROR;
            goto ExitFunction;
        }

        *PowerState = USB_PWR_CTRL_ANX74XX_POWER_ON_EVENT;

        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a ANX Power-On Config Done \r\n", __FUNCTION__));

        if((intr_alert_1 & ANX74XX_ALERT_1_INTP_POWER_OFF_INTR) == 0)
            goto ExitFunction;
    }

    if(intr_alert_1 & ANX74XX_ALERT_1_INTP_POWER_OFF_INTR)
    {
        /* Reset OCM - OCM_RESET: 1 */
        Status = anx74xx_ocm_control(&gAnx74xxDevice, ANX74XX_OCM_CTRL_OCM_RESET, TRUE);

        /* CABLE_DET_DIG: 0 and DIGITAL_RDY: 1 */
        mask = 0xC0; data = 0x80;
        i2c_sts  = anx74xx_write_byte_mask(&Anx74xxTcpcDevice, ANALOG_CTRL_10, mask, data, ANX_DATA_SIZE);

        USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Current TypeCPortRole: %d \r\n", __FUNCTION__, gTypeCPortRole));
        /* Role: Set as Current Role for next Power-on, CC1/CC2: Rd and/or Rp(default) */
        switch(gTypeCPortRole)
        {
          case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK:
            data = 0x0A;  /* Rd/Rd */
            break;
          case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SOURCE:
            data = 0x05;  /* Rp/Rp */
            break;
          case EFI_USB_PWR_CTRL_TYPEC_POWERROLE_DUAL:
            data = 0x4A;  /* Rd or Rp(default) */
            break;
          default:
            data = 0x0F;  /* Open */
        }
        i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, ROLE_CONTROL, &data, ANX_DATA_SIZE);

        i2c_sts |= UsbPwrCtrl_i2c_read(&gAnx74xxDevice, HW_CHIP_REV, (UINT8 *)&hw_chip_rev, ANX_DATA_SIZE);
        if(hw_chip_rev == AA_CHIP_REV)
        {
            /* R_TOGGLE_ENABLE: Enable Analog Toggle, TOGGLE_CTRL_MODE: Auto control */
            mask = data = 0xA0;
            i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, ANALOG_CTRL_1, &data, ANX_DATA_SIZE);

            /* R_LATCH_TOGGLE_ENABLE: R_TOGGLE_ENABLE valid */
            mask = data = 0x40;
            i2c_sts |= anx74xx_write_byte_mask(&Anx74xxTcpcDevice, ANALOG_CTRL_1, mask, data, ANX_DATA_SIZE);
        }
        else if(hw_chip_rev == AC_CHIP_REV)
        {
            data = 0x99;
            i2c_sts |= UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, COMMAND, &data, ANX_DATA_SIZE);
        }

        if(EFI_ERROR(Status) || I2C_SUCCESS != i2c_sts)
        {
            USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a ANX Firmware Power-Off Sequence Failed \r\n", __FUNCTION__));
            Status = EFI_DEVICE_ERROR;
            goto ExitFunction;
        }

        /* Power Down (idle) ANX Chip */
        data = 0xFF;
        UsbPwrCtrl_i2c_write(&Anx74xxTcpcDevice, COMMAND, &data, ANX_DATA_SIZE);

        *PowerState = USB_PWR_CTRL_ANX74XX_POWER_OFF_EVENT;

        USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a ANX Power-Off Sequence Done \r\n", __FUNCTION__));
        goto ExitFunction;
    }

    if(intr_alert_0)
        *PowerState = USB_PWR_CTRL_ANX74XX_NON_POWER_EVENT;

ExitFunction:
    return Status;
}
