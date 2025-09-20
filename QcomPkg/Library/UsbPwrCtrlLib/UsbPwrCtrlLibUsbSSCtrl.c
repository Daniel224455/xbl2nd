/**
@file  UsbPwrCtrlLibUsbSSCtrl.c
@brief Implementation of UsbPWrCtrlLibrary API for USB SS Control Re-driver
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
08/29/17   mr      Initial Release
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
#include "UsbPwrCtrlLibUsbSSCtrl.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS USBSSCtrl_PreInit(EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *USBSSCtrlDep);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;

/* USB SS Re-driver config
Power Role  DIR0        DIR1    CTL0            CTL1            FLIP        Configuration
            (MSM GPIO7)         (MSM GPIO51)    (MSM GPIO52)    (MSM GPIO6)
NA          Don't Care  NC      L               L               Don't Care  Power Down, Cable Mode
Source      L           NC      H               L               L           USB mode, No FLIP (CC1)
Source      L           NC      H               L               H           USB mode, FLIP (CC2)
Sink        H           NC      H               L               L           USB mode, No FLIP (CC1)
Sink        H           NC      H               L               H           USB mode, FLIP (CC2)
*/
USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG UsbPwrCtrlLib_USBSSCtrlModeConfig[USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_MAX] =
{
    [USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_DOWN].UsbSSCtrlModeConfig =
        {
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW
        },
    [USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SRC_UNFLIPPED].UsbSSCtrlModeConfig =
        {
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW
        },
    [USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SRC_FLIPPED].UsbSSCtrlModeConfig =
        {
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH
        },
    [USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SNK_UNFLIPPED].UsbSSCtrlModeConfig =
        {
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW
        },
    [USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SNK_FLIPPED].UsbSSCtrlModeConfig =
        {
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH
        }
};

/**
  Initialize USB SS Control Dependencies

  @param HwInfo                 Refer EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_Init(EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO USBSSCtrl)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = USBSSCtrl_PreInit(USBSSCtrl.SSCtrlExtDep);
    if(EFI_ERROR(Status)) {return Status;}

    USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a USBSSCtrl_Init Success \r\n", __FUNCTION__));

    return Status;
}

/**
  De-initialize USB SS Control Dependencies

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_Deinit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    return Status;
}


/**
  Set USB SS Dependencies based on USB Port connect

  @param USBSSCtrlDep           Refer EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP for details
  @param USBSSCtrlModeConfig    Refer USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_SetModeConfig(
  EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *USBSSCtrlDep,
  USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE USBSSCtrlModeConfig)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8 NumDep = 0;
    USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG *pUSBSSCtrlModeConfig = NULL;

    if(NULL == TLMMProtocol) {return EFI_NOT_READY;}
    if(USBSSCtrlModeConfig >= USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_MAX) {return EFI_INVALID_PARAMETER;}

    pUSBSSCtrlModeConfig = &UsbPwrCtrlLib_USBSSCtrlModeConfig[USBSSCtrlModeConfig];

    for(NumDep = 0; NumDep < USB_PWR_CTRL_USB_SSCTRL_DEP_MAX; NumDep++)
    {
        switch(USBSSCtrlDep[NumDep].ExtDepResource)
        {
          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE:
            {
              Status |= TLMMProtocol->GpioOut(
                          (UINT32)EFI_GPIO_CFG(USBSSCtrlDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          ((EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH == pUSBSSCtrlModeConfig->UsbSSCtrlModeConfig[NumDep]) ? GPIO_HIGH_VALUE : GPIO_LOW_VALUE));
            }
            break;

          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_PMIC_RESOURCE:
          default:
            continue;
        }
    }

    return Status;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTION DEFINITIONS                            */
/*===========================================================================*/
EFI_STATUS
USBSSCtrl_PreInit(EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *USBSSCtrlDep)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8 NumDep = 0;

    if(NULL == USBSSCtrlDep) {return EFI_INVALID_PARAMETER;}

    Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void**)&TLMMProtocol);
    if(EFI_ERROR(Status) || NULL == TLMMProtocol)
	{
		USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    for(NumDep = 0; NumDep < USB_PWR_CTRL_USB_SSCTRL_DEP_MAX; NumDep++)
    {
        switch(USBSSCtrlDep[NumDep].ExtDepResource)
        {
          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE:
            {
              Status  = TLMMProtocol->ConfigGpio(
                          (UINT32)EFI_GPIO_CFG(USBSSCtrlDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          TLMM_GPIO_ENABLE);

              Status |= TLMMProtocol->GpioOut(
                          (UINT32)EFI_GPIO_CFG(USBSSCtrlDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          ((EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH == USBSSCtrlDep[NumDep].InitState) ? GPIO_HIGH_VALUE : GPIO_LOW_VALUE));

              if(EFI_ERROR(Status))
			  {
				  USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a TLMMProtocol APIs Failed, Status [%r] \r\n", __FUNCTION__, Status));
				  return Status;
			  }

              USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Resource[%d] Enable Success \r\n", __FUNCTION__, USBSSCtrlDep[NumDep].ResourceID));
              // gBS->Stall((uint32)(1000));  /* 1 Msec wait */
            }
            break;

          case EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_PMIC_RESOURCE:
          default:
            continue;
        }
    }

    return Status;
}
