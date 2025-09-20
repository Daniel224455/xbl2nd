/** @file

  The public header file for UsbConfigUsbCore.c

**/
//============================================================================
/**
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================

/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 02/13/18   gk	     Port changes from boot.xf.2.0 and boot.xf.2.2
 12/1/17    pm       Code Clean-up
 02/18/16   ck       Re-arrange data structures.
=============================================================================*/

#ifndef _USB_CONFIG_USB_PORTS_PRIV_H_
#define _USB_CONFIG_USB_PORTS_PRIV_H_

#include "UsbConfigUsbPortsPublic.h"

extern EFI_USB_PWR_CTRL_PROTOCOL              *gPwrCtrlProtocol;
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

