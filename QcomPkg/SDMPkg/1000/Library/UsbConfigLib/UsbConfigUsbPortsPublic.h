/** @file \UsbConfigUsbPortsPublic.h

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
 02/13/17`  gk	     Port changes from boot.xf.2.0 and boot.xf.2.2
 10/20/17   gk       Updated new Pmic API for UsbPwrCtrlDxe for UEFI
 02/03/17   pm       Port from Boot.xf.1.4
 02/18/16   ck       Init check-in
=============================================================================*/

#ifndef _USB_CONFIG_USB_PORTS_PUB_H_
#define _USB_CONFIG_USB_PORTS_PUB_H_

#include "UsbConfigLibPublic.h"

typedef struct _QCOM_USB_PORT_INTERFACE QCOM_USB_PORT_INTERFACE;

/**
  USB port id. 
*/
typedef enum {
  USB_PORT_0 = 0,      /**< Port0. */
  USB_PORT_1,          /**< Port1. */
  USB_PORT_2,          /**< Port2. */
  USB_PORT_3,          /**< Port3. */  
  USB_PORT_MAX_NUM     /**< Invalid Port. */
} QCOM_USB_PORT_NUM;

//
// The possible USB Type-C port mode.
// 
typedef enum _USB_TYPEC_PORT_MODE
{
  USB_TYPEC_PORT_MODE_NONE,
  USB_TYPEC_PORT_MODE_UFP,
  USB_TYPEC_PORT_MODE_DFP,
  USB_TYPEC_PORT_MODE_INVALID
} USB_TYPEC_PORT_MODE;


//
// The possible USB Type-C CC out status.
// 
typedef enum _USB_TYPEC_CC_OUT_STATUS
{
  USB_TYPEC_CC_OUT_RESERVED,
  USB_TYPEC_CC_OUT_CC1,
  USB_TYPEC_CC_OUT_CC2,
  USB_TYPEC_CC_OUT_OPEN,
  USB_TYPEC_CC_OUT_INVALID
} USB_TYPEC_CC_OUT_STATUS;

// 
// Payload for the current message.
//
typedef union _USB_TYPEC_PORT_MESSAGE_PAYLOAD
{
  USB_TYPEC_PORT_MODE UsbTypeCPortMode;
} USB_TYPEC_PORT_MESSAGE_PAYLOAD;

// 
// Event message.
// 
typedef enum _USB_CFG_PORT_MESSAGE
{
  UsbCfgPortMsgNone = 0,
  /**< No messages. */
  UsbCfgPortMsgPartnerDetach,
  /**< Detach event was signaled. */
  UsbCfgPortMsgPartnerAttach,
  /**< Attach event was signaled. */
} /** @cond */USB_CFG_PORT_MESSAGE/** @endcond */;

//
// USB Type-C Port Connection State
//
typedef enum {
  USBC_STATE_INVALID = 0,   // Invalid Connection State
  USBC_STATE_DETACHED,      // Detached Connection State
  USBC_STATE_ATTACHED,      // Attached connection State
  USBC_STATE_MAX
} USB_CFG_PORT_CONNECT_STATE;

typedef struct _QCOM_USB_PORT_CFG {
  QCOM_USB_CORE_NUM         Core;
  QCOM_USB_CONNECTOR_TYPE   Type;
} QCOM_USB_PORT_CFG;

/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/

/**
@brief Initialize a USB Type-C port specified by the CoreNum.

The client shall call the function to get the USB port handle before calling 
any other public interfaces exposed by the module. 

@param [in]  CoreNum          The core number associated with the port to be 
                              initialized. 
@param [out] UsbPortHandle    The USB port handle associated with the port

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/
EFI_STATUS
UsbConfigPortsInit(
  IN  UINTN      CoreNum,
  OUT VOID     **UsbPortHandle
  );

  
/**
@brief De-initialize a USB Type-C port.

After calling the function, the client shall not invoke any other APIs 
exposed by the module. 

@param [in] UsbPortHandle    The USB port handle associated with the port

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/
EFI_STATUS
UsbConfigPortsDeinit(
  IN VOID     *UsbPortHandle
  );

  
/**
@brief Event handler function for a USB port.

The client calls the function periodically to obtain the new Type-C related events 
associated with the USB port. 

@param [in]     UsbPort         The handle of the port
@param [out]    Message         The message generated based on the event
@param [in,out] PayloadSize     The size of Payload associated with the message
@param [out]    Payload         The payload associated with the message.
@param [in]     CoreNum         The CoreNum associated with the core number/port index.

@retval EFI_SUCCESS       The operation succeeded.
@retval Others            The operation failed.
**/
EFI_STATUS
UsbConfigPortsEventHandler(
  IN     VOID                            *UsbPortHandle,
  OUT    USB_CFG_PORT_MESSAGE            *Message,
  IN OUT UINTN                           *PayloadSize,
  OUT    USB_TYPEC_PORT_MESSAGE_PAYLOAD  *Payload,
  IN     UINT8                            CoreNum
  );


/**
@brief Return the real-time connection state for an USB Type-C port. 

The function returns the real-time value of the type-c port mode value 
associated with the UsbPortHandle.

@param [in]  UsbPortHandle    The handle of the USB port
@param [out] UsbPortMode      The latest cached USB_TYPEC_PORT_MODE value
@param [in]  CoreNum          The CoreNum associated with the core number/port index.

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/
EFI_STATUS
UsbPortGetRtConnectionMode(
  IN  VOID                *UsbPortHandle,
  OUT USB_TYPEC_PORT_MODE *UsbPortMode,
  IN  UINT8                CoreNum
  );


/**
@brief Return the cached connection state for an USB Type-C port. 

The function returns the cached value of the type-c port mode 
value associated with the UsbPortHandle. The value is only
updated, if any, when the client calls the EventHandler, which
queries the hardware for the update.

@param [in]  UsbPortHandle    The handle of the USB port
@param [out] UsbPortMode      The latest cached USB_TYPEC_PORT_MODE value

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/
EFI_STATUS
UsbPortGetConnectionMode(
  IN  VOID                *UsbPortHandle,
  OUT USB_TYPEC_PORT_MODE *UsbPortMode
  );


/**
@brief Return the cached CC out status for an USB Type-C port.

The function returns the cached value of the type-c port CC out
status associated with the UsbPortHandle. The value is only
updated, if any, when the client calls the EventHandler, which 
queries the hardware for the update.

@param [in]  UsbPortHandle    The handle of the USB port
@param [out] CcOutStatus      The CC out status returned by the hardware

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/
EFI_STATUS
UsbPortGetCcOutStatus(
  IN  VOID                    *UsbPortHandle,
  OUT USB_TYPEC_CC_OUT_STATUS *CcOutStatus
  );

/**
@brief Return the cached core number and connector type for an USB Type-C port.

The function returns the cached value of core number and connector type 
associated with the port index. 

@param [in]  PortIndex      Index of the port to be queried.
@param [out] ConnectorType  Physical connector type of the hardware
@param [out] CoreNum        Core number to which port is connected in hardware.

@retval EFI_SUCCESS           The operation succeeded.
@retval Others                The operation failed.
**/

EFI_STATUS
UsbPortGetInfo(
  IN     UINT8                    PortIndex,
  OUT    QCOM_USB_CORE_NUM        *CoreNum,
  OUT    QCOM_USB_CONNECTOR_TYPE  *ConnectorType  
  );

#endif /* _USB_CONFIG_USB_PORTS_PUB_H_ */
