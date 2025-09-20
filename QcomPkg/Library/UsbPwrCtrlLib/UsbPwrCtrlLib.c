/**
@file  UsbPwrCtrlLib.c
@brief Provide Access to UsbPWrCtrlLibrary API
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
12/11/17   dc      Remove CLS Platform related config
08/03/17   mr      Added support for ANX and ROHM Libs
07/13/17   dc      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>

/**
  Library Dependencies
*/
#include <Library/QcomLib.h>

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"

#include "UsbPwrCtrlLibPmSchg.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS UsbPwrCtrlLib_ValidateRequest(UINT8 PortIndex);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
EFI_USB_PWR_CTRL_HW_INFO *pUsbPwrCtrlHwInfo = NULL;

/*===========================================================================*/
/*                  FUNCTION DEFINITIONS                                      */
/*===========================================================================*/

/**
  Initialize UsbPwrCtrl Library functions

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_Init()
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType;

  //Get Hardware Info based on PlatformType
  Status = GetPlatformType(&PlatformType);
  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Get platform type Error[%r] \r\n"
                          , __FUNCTION__, Status));
    return Status;
  }

  Status  = UsbPwrCtrlLibConfig_Init(PlatformType);
  Status |= UsbPwrCtrlLibConfig_GetHWInfo(&pUsbPwrCtrlHwInfo);
  if(EFI_ERROR(Status) || pUsbPwrCtrlHwInfo == NULL)
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Initialize Hardware Configuration Error[%r] \r\n"
                          , __FUNCTION__, Status));
    return Status;
  }

  USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a SUCCESS \r\n",  __FUNCTION__));
  return Status;
}


/**
  Get USB Port Hw information

  @param[OUT]   pUsbPortHwInfo  Refer EFI_USB_PWR_CTRL_USB_PORTHW_INFO for more details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetHwInfo(
  EFI_USB_PWR_CTRL_USB_PORTHW_INFO *pUsbPortHwInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //Sanity Check
  if(NULL == pUsbPortHwInfo) {return EFI_INVALID_PARAMETER;}

  pUsbPortHwInfo->NumOfPorts = pUsbPwrCtrlHwInfo->NoOfPorts;

  return Status;
}

/**
  Get Vbus Detect result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetVbusDetect(
  UINT8 PortIndex,
  BOOLEAN *bVbusDetect)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)) {return Status;}

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_GetVbusDetect(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwInfo, bVbusDetect);
      break;
    //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
    //  break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_ROHM:
      Status = ChargerExtHwLib_GetVbusDetect(bVbusDetect);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Get Vbus Source OK result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bVbusSourceOK   Return the result of Vbus SourceOK

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetVbusSourceOK(
  UINT8 PortIndex,
  BOOLEAN *bVbusSourceOK)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  if(NULL == bVbusSourceOK) {return EFI_INVALID_PARAMETER;}

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_GetVbusSourceOK(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwInfo, bVbusSourceOK);
      break;
    //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
    //  break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_NONE:
      *bVbusSourceOK = TRUE;
      Status = EFI_SUCCESS;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Get HSUSB Charger Port Type

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetHSUsbChgPortType(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *ChgPortType)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  if(NULL == ChgPortType) {return EFI_INVALID_PARAMETER;}

  *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN;

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_GetHSUsbPortType(pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwInfo, ChgPortType);
      break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_ROHM:
      {
        CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE ChgrPortType;

        Status = ChargerExtHwLib_GetHSUsbPortType(&ChgrPortType);
        if(EFI_ERROR(Status)) {return Status;}

        switch(ChgrPortType)
        {
          case CHARGER_EXT_HW_HSUSB_CHGPORT_SDP:
            *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP;
            break;
          case CHARGER_EXT_HW_HSUSB_CHGPORT_CDP:
            *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP;
            break;
          case CHARGER_EXT_HW_HSUSB_CHGPORT_DCP:
            *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP;
            break;
          case CHARGER_EXT_HW_HSUSB_CHGPORT_PULLUP:
          case CHARGER_EXT_HW_HSUSB_CHGPORT_OPEN:
            *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_OCP;
            break;
          default:
            break;
        }
      }
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Get USB ID result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bUsbID          Return USB ID Status

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetUsbIDStatus(
  UINT8 PortIndex,
  BOOLEAN *bUsbID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].UsbIdDetect.HwType)
  {
    //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
    //  break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Get TypeC Port Status

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetTypeCPortStatus(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)) {return Status;}

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_GetTypeCPortStatus(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwInfo, TypeCPortStatus);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Get Port PD Status

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   PDStatus        Return PD Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetPDStatus(
    UINT8 PortIndex,
    EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)) {return Status;}

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType)
  {
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Set Sink Max Input Current

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetSnkMaxInputCurrent(
  UINT8 PortIndex,
  UINT32 MaxCurrentMA)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Current = %d mA\r\n",  __FUNCTION__, MaxCurrentMA));

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_SetSnkMaxInputCurrent(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwInfo, MaxCurrentMA);
      break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_ROHM:
      Status = ChargerExtHwLib_SetSnkMaxInputCurrent(MaxCurrentMA);
      break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
      Status = EFI_SUCCESS;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Set Source Max Output Current

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    MaxCurrentMA    Value to set Max Output Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetSrcMaxOutputCurrent(
  UINT8 PortIndex,
  UINT32 MaxCurrentMA)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Current = %d mA\r\n",  __FUNCTION__, MaxCurrentMA));

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSource.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_SetSrcMaxOutputCurrent(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSource.HwInfo, MaxCurrentMA);
      break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
      Status = EFI_SUCCESS;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Enable/Disable Vbus

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    bVbusEn         Enable/Dsiable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetVbusEn(
  UINT8 PortIndex,
  BOOLEAN bVbusEn)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a VbusEn = %d\r\n",  __FUNCTION__, bVbusEn));

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_SetVbusEn(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwInfo, bVbusEn);
      break;
    //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
    //  break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
      Status = EFI_SUCCESS;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}
/**
  Enable/Disable Vconn

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    bVconnEn        Enable/Dsiable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetVconnEn(
  UINT8 PortIndex,
  BOOLEAN bVconnEn)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a VconnEn = %d\r\n",  __FUNCTION__, bVconnEn));

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VconnCtrl.HwType)
  {
    case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
      Status = UsbPwrCtrlLibPmSchg_SetVconnEn(pUsbPwrCtrlHwInfo->Ports[PortIndex].VconnCtrl.HwInfo, bVconnEn);
      break;
    //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
    //  break;
    case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
      Status = EFI_SUCCESS;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Validate the Request

  @param[IN]    PortIndex       Specify which port the request is for

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_NOT_READY:                The physical device does not support this function

*/


/*===========================================================================*/
/*                  INTERNAL FUNCTION DEFINITIONS                            */
/*===========================================================================*/
EFI_STATUS
UsbPwrCtrlLib_ValidateRequest(UINT8 PortIndex)
{
  if(pUsbPwrCtrlHwInfo == NULL)
  {
    return EFI_NOT_READY;
  }

  if(PortIndex >= pUsbPwrCtrlHwInfo->NoOfPorts)
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Invalid Port Index = %d, expected < %d\r\n"
                          , __FUNCTION__, PortIndex, pUsbPwrCtrlHwInfo->NoOfPorts));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

