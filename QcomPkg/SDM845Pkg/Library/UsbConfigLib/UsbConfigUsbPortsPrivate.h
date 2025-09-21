/** @file

  The public header file for UsbConfigUsbCore.c
  Copyright (c) 2016-2017 QUALCOMM Technologies Inc. All rights reserved.

**/


/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 12/1/17    pm       Code Clean-up
 02/18/16   ck       Re-arrange data structures.
=============================================================================*/

#ifndef _USB_CONFIG_USB_PORTS_PRIV_H_
#define _USB_CONFIG_USB_PORTS_PRIV_H_

#include "UsbConfigUsbPortsPublic.h"

extern EFI_USB_PWR_CTRL_PROTOCOL              *gPwrCtrlProtocol;;
extern EFI_PLATFORMINFO_PLATFORM_INFO_TYPE    PlatformInfo;

/**
Structure encapsulating a USB port. Contains port specific state information
**/
typedef struct _QCOM_USB_PORT {
  USB_CFG_PORT_MESSAGE                 Message;
  USB_CFG_PORT_CONNECT_STATE           ConnectState;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS   PortStatus;
} QCOM_USB_PORT;

#endif /* _USB_CONFIG_USB_PORTS_PRIV_H_ */

