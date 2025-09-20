/**
@file  UsbPwrCtrlLib.c
@brief Provide Access to UsbPWrCtrlLibrary API
*/
/*=============================================================================
Copyright (c) 2017-2019 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
02/06/19   dc      remove unnecessary error print for PD Status API
01/25/19   dc      Support PD Status API
10/21/18   ivy     Updated pmicglink to polling-based and support fetching usb info from charger FW
06/18/18   dc      Support Dynamic Port Detection
06/18/18   dc      Add port indexes support in GetHwInfo API 
04/17/18   dc      Update Non-Error Debug Message Level to INFO 
04/17/18   dc      Enable Support for Type-A Port
04/06/18   dc      Change recovery mode message level from WARN to INFO 
02/20/18   dc      Update for SDM1000
11/14/17   ra      Changes pertaining to PM855B
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
#include <Library/QcomBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"
#include "UsbPwrCtrlLibPmUcsi.h"
#include "UsbPwrCtrlLibPmSchg.h"
#include "UsbPwrCtrlLibPmGpio.h"
#include "UsbPwrCtrlLibUsbSSCtrl.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS UsbPwrCtrlLib_ValidateRequest(UINT8 PortIndex);
EFI_STATUS UsbPwrCtrlLib_Post_Init();
void UsbPwrCtrlLibConfig_Detect_Hw();

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

  UsbPwrCtrlLibConfig_Detect_Hw();

  Status = UsbPwrCtrlLib_Post_Init();
  USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a SUCCESS \r\n",  __FUNCTION__));
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
  if(NULL == pUsbPortHwInfo)
  {return EFI_INVALID_PARAMETER;}

  pUsbPortHwInfo->NumOfPorts = pUsbPwrCtrlHwInfo->NoOfPorts;
  pUsbPortHwInfo->PortIndexes = pUsbPwrCtrlHwInfo->PortIndexes;

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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;

  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status))
  {return Status;}

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex,
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_GetVbusDetect(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwInfo, bVbusDetect);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI:
        Status = UsbPwrCtrlLibPmUcsi_GetVbusDetect(PortIndex, bVbusDetect);
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusDetect.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  if(NULL == bVbusSourceOK) {return EFI_INVALID_PARAMETER;}

  //For Type-A port, always return True for VbusSourceOK 
  if(pUsbPwrCtrlHwInfo->Ports[PortIndex].ConnectorType == EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA)
  {
    *bVbusSourceOK = TRUE;
    return Status;
  }

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_GetVbusSourceOK(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwInfo, bVbusSourceOK);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI:
        Status = UsbPwrCtrlLibPmUcsi_GetVbusSourceOK(PortIndex, bVbusSourceOK);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_NONE:
        *bVbusSourceOK = TRUE;
        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusSrcOkDetect.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  if(NULL == ChgPortType) {return EFI_INVALID_PARAMETER;}

  *ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN;

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_GetHSUsbPortType(pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwInfo, ChgPortType);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI:
        Status = UsbPwrCtrlLibPmUcsi_GetHSUsbPortType(PortIndex, ChgPortType);
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].Usb2ChgDetect.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].UsbIdDetect.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].UsbIdDetect.HwType[HwTypeIndex])
    {
      //case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
      //  break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].UsbIdDetect.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)) {return Status;}

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_GetTypeCPortStatus(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwInfo, TypeCPortStatus);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI:
        Status = UsbPwrCtrlLibPmUcsi_GetTypeCPortStatus(PortIndex, TypeCPortStatus);
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)) {return Status;}

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI:
        Status = UsbPwrCtrlLibPmUcsi_GetPDStatus(PortIndex, PDStatus);
        break;      
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        PDStatus->bPDConsumer = FALSE;
        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].TypeC.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Current = %d mA\r\n",  __FUNCTION__, PortIndex, MaxCurrentMA));

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwType[HwTypeIndex]));
    }

    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_SetSnkMaxInputCurrent(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwInfo, MaxCurrentMA);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
      break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSink.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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

  USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Current = %d mA\r\n",  __FUNCTION__, PortIndex, MaxCurrentMA));

  switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].PowerSource.HwType[0])
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
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: VbusEn = %d\r\n",  __FUNCTION__, PortIndex, bVbusEn));

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwType[HwTypeIndex]));
    }
    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwType[HwTypeIndex])
    {
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG:
        Status = UsbPwrCtrlLibPmSchg_SetVbusEn(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwInfo, bVbusEn);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO:
        Status = UsbPwrCtrlLibPmGpio_SetVbusEn(pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwInfo, bVbusEn);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_HW_AUTO:
        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
    }
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].VbusCtrl.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE HwTypeIndex = EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY;
  //Sanity Check
  Status = UsbPwrCtrlLib_ValidateRequest(PortIndex);
  if(EFI_ERROR(Status)){return Status;}

  USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: VconnEn = %d\r\n",  __FUNCTION__, PortIndex, bVconnEn));

  do
  {
    if(HwTypeIndex != EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY)
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Pri. Config Failed, Use %d, \r\n", __FUNCTION__, PortIndex, 
                            pUsbPwrCtrlHwInfo->Ports[PortIndex].VconnCtrl.HwType[HwTypeIndex]));
    }

    switch(pUsbPwrCtrlHwInfo->Ports[PortIndex].VconnCtrl.HwType[HwTypeIndex])
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
  }
  while(EFI_ERROR(Status) &&
        (++HwTypeIndex < EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX) &&
        (pUsbPwrCtrlHwInfo->Ports[PortIndex].VconnCtrl.HwType[HwTypeIndex] != EFI_USB_PWR_CTRL_PORTHW_HW_NONE));

  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Failed, Error[%r] \r\n", __FUNCTION__, PortIndex, Status));
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
  uint8 valid_port_index = 0;

  if(pUsbPwrCtrlHwInfo == NULL)
  {
    return EFI_NOT_READY;
  }

  for (valid_port_index = 0; valid_port_index < pUsbPwrCtrlHwInfo->NoOfPorts; valid_port_index++)
  {
    if(PortIndex == pUsbPwrCtrlHwInfo->PortIndexes[valid_port_index])
    {
      //PortIndex is valid (listed in PortIndexes structure)
      return EFI_SUCCESS;
    }
  }

  USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Invalid Port Index = %d\r\n"
                          , __FUNCTION__, PortIndex));

  return EFI_INVALID_PARAMETER;
}
EFI_STATUS 
UsbPwrCtrlLib_Post_Init()
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 index  = 0;

  /*Turn On Vbus for all Type-A Port*/
  for (index = 0; index < pUsbPwrCtrlHwInfo->NoOfPorts; index++)
  {
    if (pUsbPwrCtrlHwInfo->Ports[index].ConnectorType == EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA)
    {
      Status = UsbPwrCtrlLib_SetVbusEn(index, TRUE);
      if (Status != EFI_SUCCESS)
      {
        return Status;
      }
    }
  }
  return Status;
}
void
UsbPwrCtrlLibConfig_Detect_Hw()
{
  EFI_STATUS status = EFI_SUCCESS;
  uint8 port_index = 0;
  uint8 valid_port_index = 0;
  uint8 ValidPortIndexes[EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO];
  BOOLEAN bVbusDetect;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS typec_port_status;

  SetMem (ValidPortIndexes,EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO, 0x00);

  for (port_index = 0; port_index < pUsbPwrCtrlHwInfo->NoOfPorts; port_index++)
  {
    switch (pUsbPwrCtrlHwInfo->Ports[port_index].ConnectorType)
    {
      case EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_uUSB:
        //Use VbusDetect API to test uUSB port
        status = UsbPwrCtrlLib_GetVbusDetect(port_index,&bVbusDetect);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC:
        //Use TypeC Status API to test Type-C port
        status = UsbPwrCtrlLib_GetTypeCPortStatus(port_index, &typec_port_status);
        break;
      case EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA:
        //No need to test Type-A port
        status = EFI_SUCCESS;
        break;
      default:
        //Invalid Port Type
        status = EFI_UNSUPPORTED;
    }

    if(EFI_ERROR(status))
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Port[%d]: Detection Failed, Error[%r] \r\n", __FUNCTION__, port_index, status));
    }
    else
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Port[%d]: Detected\r\n", __FUNCTION__, port_index));
      ValidPortIndexes[valid_port_index] = port_index;      
      valid_port_index++;
    }
  }

  if(valid_port_index != pUsbPwrCtrlHwInfo->NoOfPorts)
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a NoOfPort(s): Expected = %d, Detected = %d\r\n", 
                          __FUNCTION__, pUsbPwrCtrlHwInfo->NoOfPorts, valid_port_index));
  }

  //Update No Of Port and Port Indexes;
  pUsbPwrCtrlHwInfo->NoOfPorts = valid_port_index;
  CopyMemS(pUsbPwrCtrlHwInfo->PortIndexes,EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO,
           ValidPortIndexes, EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO);
}

