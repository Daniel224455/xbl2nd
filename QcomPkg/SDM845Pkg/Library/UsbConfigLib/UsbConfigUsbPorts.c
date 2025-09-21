/** @file \UsbConfigUsbPorts.c

  Implement USB Type-C related functionality for the platform.

  Copyright (c) 2016-2017 QUALCOMM Technologies Inc. All rights reserved.
**/


/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 11/13/17   pm       Added support for device mode on secondary port
 11/14/17   gk       Enabling Device Mode on CLS_PlatForm
 10/20/17   gk       Updated new Pmic API for UsbPwrCtrlDxe for UEFI
 06/14/17   amitg    Cleanup
 04/04/17   pm       Added check for USB connector type in UsbConfigPortsQueryConnectionChange
 03/16/17   pm       Changed DBG Log level from WARN to VERBOSE for RUMI Platform 
                     in UsbConfigPortsQueryConnectionChange API  
 03/06/17   amitg    EUD Support
 02/03/17   pm       Port from Boot.xf.1.4
 12/20/16   sm       Edited input parameter list for UsbCGetConnectState and 
                     UsbCGetPortState PmicUsbProtocol API
 09/22/16   amitg    Napali Pre-Si Updates
 03/31/16   ck       Only free UsbPortHandle if allocated
 02/18/16   ck       Init Version
=============================================================================*/

#include "UsbConfigUsbPortsPrivate.h"

BOOLEAN SimulateDetach = FALSE;
BOOLEAN SimulateHostCableAttach = FALSE;
BOOLEAN SimulatePeriCableAttach = FALSE;

/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbConfigPortsInit(
  IN  UINTN      CoreNum,
  OUT VOID     **UsbPortHandle
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_PORT *Port;

  if (!UsbPortHandle) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: UsbPortHandle %p", UsbPortHandle);
  }

  Port = AllocateZeroPool(sizeof(QCOM_USB_PORT));
  if (!Port) {
    Status = EFI_OUT_OF_RESOURCES;
    ERR_CHK("Out of resources");
  }

  Port->Message = UsbCfgPortMsgNone;
  Port->ConnectState = USBC_STATE_DETACHED;
  Port->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN;

  *UsbPortHandle = Port;

ON_EXIT:
  return Status;
}

/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbConfigPortsDeinit(
  IN VOID     *UsbPortHandle
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  if (UsbPortHandle)
    FreePool(UsbPortHandle);
  return Status;
}


/**
@Brief - Print the fields in EFI_PM_USB_TYPEC_PORT_STATUS

@param [in]     PrintFlag          Debug level flag
@param [in]     UsbPort            An instance of QCOM_USB_PORT.
@param [in]     Message            The message generated based for attach / detach.
@param [in]     CoreNum            The CoreNum associated with the core number/port index.


**/
__inline
static
VOID 
UsbConfigPortsPrintPortStatus (
  IN    UINT32                PrintFlag,
  IN    QCOM_USB_PORT         *UsbPort,
  IN    USB_CFG_PORT_MESSAGE  Message,
  IN    UINT8                 CoreNum
  
  )
  {
    switch (Message)
    { 
      case UsbCfgPortMsgPartnerAttach:
        DEBUG((PrintFlag, "ConnectSts : Attach, "));
        break;
      case UsbCfgPortMsgPartnerDetach:
        DEBUG((PrintFlag, "ConnectSts : Detach, "));
        break;
      default:
        DEBUG((PrintFlag, "ConnectSts : None, "));
        break;
    }
    
    switch (UsbPort->PortStatus.DataRole)
    {
      case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP:
        DEBUG((PrintFlag,
        "Data Role : UFP (DEVICE Mode), "));
        break;
      case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP:
        DEBUG((PrintFlag,
        "Data Role  : DFP (HOST Mode), "));
        break;
      case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN:
        DEBUG((PrintFlag,
        "Data Role : UNKNOWN, "));
        break;
      default:
        DEBUG((PrintFlag,
        "Data Role : INVALID, "));
        break;
     }

    if (CoreNum == USB_CORE_0)
    {
      switch (UsbPort->PortStatus.CCOrientation)
      {
        case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_UNORIENTED:
          DEBUG((PrintFlag,
          "Lane : UNORIENTED, "));
          break;
        case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1:
          DEBUG((PrintFlag,
          "Lane : CC1, "));
          break;
        case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2:
          DEBUG((PrintFlag,
          "Lane : CC2, "));
          break;
        case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_OPEN:
          DEBUG((PrintFlag,
          "Lane : OPEN, "));
          break;
        default:
          DEBUG((PrintFlag,
          "Lane : INVALID, "));
          break;
      }
    }
    
    DEBUG((PrintFlag, "CoreNum : %d\n", CoreNum));
  
}


/**
Query USB connection change.

The function generates messages of UsbCfgPortMsgPartnerAttach and
UsbCfgPortMsgPartnerDetach if the cable attach / detach happens.

@param [in]     UsbPort         The port to be queried
@param [out]    Message         The message generated based for attach / detach
@param [in,out] PayloadSize     The size of Payload associated with the message
@param [out]    Payload         The payload associated with the message.
@param [in]     CoreNum         The CoreNum associated with the core number/port index.

@retval EFI_SUCCESS       The operation succeeded.
@retval Others            The operation failed.
**/
static
EFI_STATUS
UsbConfigPortsQueryConnectionChange(
  IN     QCOM_USB_PORT                      *UsbPort,
  OUT    USB_CFG_PORT_MESSAGE               *Message,
  IN OUT UINTN                              *PayloadSize,
  OUT    USB_TYPEC_PORT_MESSAGE_PAYLOAD     *Payload,
  IN     UINT8                              CoreNum
  ) 
{
  BOOLEAN    VbusStatus = FALSE;
  EFI_STATUS Status     = EFI_SUCCESS;
  //Initialize Message value to UsbCfgPortMsgNone 
  *Message = UsbCfgPortMsgNone;

  // Since there is no Type-C support for Pre-SI,
  // assume port in UFP mode and it is always attached.
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) ||
    (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
    (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_VERBOSE, "Pre-SI: Assume UFP and attached");

    if (UsbPort->ConnectState == FALSE) {
      UsbPort->ConnectState = TRUE;
      *Message = UsbCfgPortMsgPartnerAttach;
      *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_UFP;
      // set to CC1 for RUMI
      UsbPort->PortStatus.CCOrientation = EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1;
    }
    goto ON_EXIT;
  }

  // Make sure USB Type-C Protocol is available
  if (NULL == gPwrCtrlProtocol) {
    // Find the Power Control USB charger protocol
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
    if (EFI_ERROR(Status))
    {
      gPwrCtrlProtocol = NULL;
      ERR_CHK("Failed to open Power Control USB protocol Status =  (0x%x)", Status);
    }
  }

  if (CoreNum == USB_CORE_0) { //For primary port
    Status = gPwrCtrlProtocol->GetTypeCPortStatus(CoreNum, &UsbPort->PortStatus);
    ERR_CHK("Failed to get UsbCGetConnStateStatus");

    DBG(EFI_D_INFO, "ConnectModeStatus: %d", UsbPort->PortStatus.DataRole);
    if (SimulateDetach) {
       UsbPort->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN;
    }

    if (SimulateHostCableAttach) {
      UsbPort->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP;
    }

    if (SimulatePeriCableAttach) {
      UsbPort->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP;
    }

    if (SimulateHostCableAttach && SimulatePeriCableAttach) {
      ERR_CHK("Cannot simulate host and device at the same time");
    }
    
    switch (UsbPort->PortStatus.DataRole) {
    case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN:
      if (UsbPort->ConnectState == USBC_STATE_DETACHED)
        goto ON_EXIT;

    DBG(EFI_D_INFO, "USB Type-C Device Mode(None)");
      *Message = UsbCfgPortMsgPartnerDetach;
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_NONE;
      *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
      break;

    case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP:
      if (UsbPort->ConnectState == USBC_STATE_ATTACHED)
        goto ON_EXIT;

    DBG(EFI_D_INFO, "USB Type-C Device Mode(UFP mode)");
      *Message = UsbCfgPortMsgPartnerAttach;
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_UFP;
      *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
      break;

    case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP:
      if (UsbPort->ConnectState == USBC_STATE_ATTACHED)
        goto ON_EXIT;

    DBG(EFI_D_INFO, "USB Type-C Host Mode(DFP mode)");
      *Message = UsbCfgPortMsgPartnerAttach;
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_DFP;
      *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
      break;

    case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_INVALID:
      DBG(EFI_D_ERROR, "Invalid Typec-C connect mode detected");
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_INVALID;
      goto ON_EXIT;
      break;
      
    default:
      DBG(EFI_D_ERROR, "Unkown Typec-C connect mode detected");
      Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_INVALID;
      goto ON_EXIT;
    }
  }
  else if(CoreNum == USB_CORE_1) { //For the secondary core
    Status = gPwrCtrlProtocol->GetVbusDetectStatus(CoreNum, &VbusStatus);

    ERR_CHK("Failed to get VbusStatus");
    DBG(EFI_D_INFO, "VbusStatus: %d", VbusStatus);

    if(VbusStatus)
    {
      if (UsbPort->ConnectState == USBC_STATE_ATTACHED)
        goto ON_EXIT;
      
      if (UsbPort->ConnectState == USBC_STATE_DETACHED) {
        *Message = UsbCfgPortMsgPartnerAttach;
        *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
        Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_UFP;
        UsbPort->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP;
      }
    }
    else
    {
      if (UsbPort->ConnectState == USBC_STATE_DETACHED)
        goto ON_EXIT;
      
      if (UsbPort->ConnectState == USBC_STATE_ATTACHED) {
        *Message = UsbCfgPortMsgPartnerDetach;
        *PayloadSize = sizeof(USB_TYPEC_PORT_MODE);
        Payload->UsbTypeCPortMode = USB_TYPEC_PORT_MODE_INVALID;
        UsbPort->PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN;
      }
    }
  }

  if(*Message == UsbCfgPortMsgNone)
    goto ON_EXIT;

  // If we reach here, it means the connection status has been changed.
  if (*Message == UsbCfgPortMsgPartnerAttach) {
    UsbPort->ConnectState = USBC_STATE_ATTACHED;
  }
  else {
    UsbPort->ConnectState = USBC_STATE_DETACHED;
  }
  
  UsbConfigPortsPrintPortStatus(EFI_D_WARN, UsbPort, *Message, CoreNum);

ON_EXIT:
  return Status;
}


/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbConfigPortsEventHandler(
  IN     VOID                            *UsbPortHandle,
  OUT    USB_CFG_PORT_MESSAGE            *Message,
  IN OUT UINTN                           *PayloadSize,
  OUT    USB_TYPEC_PORT_MESSAGE_PAYLOAD  *Payload,
  IN     UINT8                            CoreNum
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_PORT *UsbPort = (QCOM_USB_PORT *)UsbPortHandle;

  FNC_ENTER_MSG();

  if (!UsbPortHandle || !Message || !PayloadSize || !Payload) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: UsbPortHandle %p, Message %p, PayloadSize %p, Payload %p",
      UsbPortHandle,
      Message,
      PayloadSize,
      Payload);
  }

  *Message = UsbCfgPortMsgNone;

  // Check if cable connection / disconnect event should be generated.
  Status = UsbConfigPortsQueryConnectionChange(
    UsbPort,
    Message,
    PayloadSize,
    Payload,
    CoreNum);

  WRN_CHK("UsbConfigPortsQueryPortConnectionUpdate failed %r", Status);

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbPortGetConnectionMode(
  IN  VOID                *UsbPortHandle,
  OUT USB_TYPEC_PORT_MODE *UsbPortMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_PORT *UsbPort = (QCOM_USB_PORT *) UsbPortHandle;

  FNC_ENTER_MSG();

  if (!UsbPortMode || !UsbPortHandle) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: UsbPortHandle %p, PortMode %p",
      UsbPortHandle,
      UsbPortMode);
  }

  switch (UsbPort->PortStatus.DataRole) {
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP:
    *UsbPortMode = USB_TYPEC_PORT_MODE_UFP;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP:
    *UsbPortMode = USB_TYPEC_PORT_MODE_DFP;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN:
    *UsbPortMode = USB_TYPEC_PORT_MODE_NONE;
    break;
  default:
    *UsbPortMode = USB_TYPEC_PORT_MODE_INVALID;
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbPortGetRtConnectionMode(
  IN  VOID                *UsbPortHandle,
  OUT USB_TYPEC_PORT_MODE *UsbPortMode,
  IN  UINT8               CoreNum
  )
{
  EFI_STATUS                           Status            = EFI_SUCCESS;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS   PortStatus = {0};
  PortStatus.DataRole = EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN;

  FNC_ENTER_MSG();

  if (!UsbPortMode || !UsbPortHandle) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: UsbPortHandle %p, PortMode %p",
      UsbPortHandle,
      UsbPortMode);
  }
  
  //Assuming USB secondary Port Connected to UFP - Hardcoded
  if (CoreNum == USB_CORE_1)
  {
    Status       = EFI_SUCCESS;
    *UsbPortMode = USB_TYPEC_PORT_MODE_UFP;
    goto ON_EXIT;
  }

  if (NULL == gPwrCtrlProtocol) {
    // Find the Power Control USB charger protocol
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
    if (EFI_ERROR(Status))
    {
      gPwrCtrlProtocol = NULL;
      WRN_CHK("Failed to open Power Control USB protocol Status =  (0x%x)", Status);
      goto ON_EXIT;
    }
  }

  Status = gPwrCtrlProtocol->GetTypeCPortStatus(CoreNum, &PortStatus);
  ERR_CHK("Failed to get UsbCGetConnStateStatus");
  DBG(EFI_D_INFO, "ConnectModeStatus: %d", PortStatus.DataRole);
  
  switch (PortStatus.DataRole) {
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP:
    *UsbPortMode = USB_TYPEC_PORT_MODE_UFP;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP:
    *UsbPortMode = USB_TYPEC_PORT_MODE_DFP;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN:
    *UsbPortMode = USB_TYPEC_PORT_MODE_NONE;
    break;
  default:
    *UsbPortMode = USB_TYPEC_PORT_MODE_INVALID;
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Check UsbConfigUsbPortsPublic.h for description. */
EFI_STATUS
UsbPortGetCcOutStatus(
  IN  VOID                    *UsbPortHandle,
  OUT USB_TYPEC_CC_OUT_STATUS *CcOutStatus
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_PORT *UsbPort = (QCOM_USB_PORT *)UsbPortHandle;

  FNC_ENTER_MSG();

  if (!UsbPortHandle || !CcOutStatus) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: UsbPortHandle %p, CcOutStatus %p",
      UsbPortHandle,
      CcOutStatus);
  }

  switch (UsbPort->PortStatus.CCOrientation)
  {
  case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_UNORIENTED:
    *CcOutStatus = USB_TYPEC_CC_OUT_RESERVED;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1:
    *CcOutStatus = USB_TYPEC_CC_OUT_CC1;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2:
    *CcOutStatus = USB_TYPEC_CC_OUT_CC2;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_OPEN:
    *CcOutStatus = USB_TYPEC_CC_OUT_OPEN;
    break;
  case EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_INVALID:
    *CcOutStatus = USB_TYPEC_CC_OUT_INVALID;
  default:
    *CcOutStatus = USB_TYPEC_CC_OUT_INVALID;
    break;
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}