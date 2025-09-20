/** @file UsbfnDwc3Lib.h
 *
 *  @brief       Platform library for the Synopsys Usbfn driver (UsbfnDwc3)
 *
 *  @details     An instance of this library provides platform specific functionallity
 *               for the UsbfnDwc3 driver in order for it to remain platform independed.
 *               This is currently only used for charger detection.
 *
 *  @ref         "DWC_usb3_databook 2.5(a)" and "HwProgrammingGuide - usb30 and SS PHY".
 *
 * 
 */
//============================================================================
/**
  Copyright (c) 2013-2014, 2018 Qualcomm Technologies, Inc. All rights reserved.
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


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
02/13/17`   gk	    Port changes from boot.xf.2.0 and boot.xf.2.2
11/13/17    pm      Added support for device mode on secondary port 
02/23/16    ck      Deprecate dci_dwc_detect_charger_type and add UsbfnDwc3GetChargerPortType
04/25/14    amitg   Charger and Cable Detection Updates for MSM 8994 (Elessar)
03/14/14    amitg   Updates for 8994
02/15/13    kameya	Initial Revision
=============================================================================*/

#ifndef USB_FN_DWC3_LIB_H
#define USB_FN_DWC3_LIB_H

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#include "UsbUrb.h"       // URB structure
#include "com_dtypes.h"   // basic types as byte word uint32 etc
#include <Library/UsbConfigLib.h>

//----------------------------------------------------------------------------
// Preprocessor Definitions and Constants
//----------------------------------------------------------------------------
#define LINESTATE_POLL_COUNT         6
#define CHG_DETECTION_LOOP_COUNT   100

// Endpoint Direction
#define QHSUSB_EP_DIR_RX   0  // OUT Token
#define QHSUSB_EP_DIR_TX   1  // IN Token

//----------------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------------

typedef struct
{
    void (*usb_reset_callback_fn)(struct qhsusb_dcd_dsc_device* dcd_context_ptr);
    void (*setup_callback_fn)(struct qhsusb_dcd_dsc_device* dcd_context_ptr, void *buffer_ptr);
    void (*port_status_changed_callback_fn)(struct qhsusb_dcd_dsc_device* dcd_context_ptr);
    void (*usb_cable_status_change_callback_fn)(struct qhsusb_dcd_dsc_device* dcd_context_ptr, boolean is_cable_attached);
    //=================================================================
    // NOTE: Trnasfer complete ok/error callback is specified in the URB.
    //=================================================================
} dci_dwc_callback_table_t;

typedef enum
{
    DCI_DWC_CHG_ST_START,
    DCI_DWC_CHG_ST_BSESS_VALID,
    DCI_DWC_CHG_ST_WAIT_FOR_DCD,
    DCI_DWC_CHG_ST_CHARGING_PORT_DETECTION,
    DCI_DWC_CHG_ST_CHARGING_PORT_DETECTED,
    DCI_DWC_CHG_ST_ACA_PORT_DETECTED,
    DCI_DWC_CHG_ST_SDP,
    DCI_DWC_CHG_ST_CDP,
    DCI_DWC_CHG_ST_DCP,
    DCI_DWC_CHG_ST_ACA_DOCK,
    DCI_DWC_CHG_ST_ACA_A,
    DCI_DWC_CHG_ST_ACA_B,
    DCI_DWC_CHG_ST_ACA_C,
    DCI_DWC_CHG_ST_INVALID_CHARGER,
    DCI_DWC_CHG_ST_DONE,
    DCI_DWC_CHG_ST_END
} dci_dwc_chg_state_type;


// Refer to Battery Charging Specification Revision 1.2
// for the definition of the following port types.
typedef enum
{
    DCI_DWC_CHG_PORT_SDP,
    DCI_DWC_CHG_PORT_CDP,
    DCI_DWC_CHG_PORT_DCP,
    DCI_DWC_CHG_PORT_ACA_DOCK,
    DCI_DWC_CHG_PORT_ACA_A,
    DCI_DWC_CHG_PORT_ACA_B,
    DCI_DWC_CHG_PORT_ACA_C,
    DCI_DWC_CHG_PORT_INVALID,
    DCI_DWC_CHG_PORT_UNKNOWN
} dci_dwc_chg_port_type;

typedef dci_dwc_chg_port_type USB_CHARGER_PORT_TYPE;

CHAR8* 
dci_dwc_print_charger_type(
  IN dci_dwc_chg_port_type charger_type
);

//============================================================================
/**
 * @function  dci_connect
 *
 * @brief Connect the device to the USB bus.
 *
 * @Note :  Connect/Disconnect is done via PMIC
 *          It can be done via the USB core when using PIPE3 PHY.
 *
 * @param CoreNum
 * @param Qscratch Base Address
 *
 * @return none
 *
 * @ref USB 3.0 Controller HPG:
 *      Chapter 4.4.2.8 - ID and VBUS override 
 *
 */
//============================================================================
void
dci_connect(
  QCOM_USB_CORE_NUM  CoreNum,
  UINTN              QscratchBase);

//============================================================================
/**
 * @function  dci_disconnect
 *
 * @brief Disconnect the device from the USB bus.
 *
 * @Note :  Connect/Disconnect is done via PMIC
 *          It can be done via the USB core when using PIPE3 PHY.
 *
 * @param CoreNum
 * @param Qscratch Base Address
 *
 * @return none
 *
 * @ref USB 3.0 Controller HPG:
 *      Chapter 4.4.2.8 - ID and VBUS override 

 *
 */
//============================================================================
void
dci_disconnect(
  QCOM_USB_CORE_NUM  CoreNum,
  UINTN              QscratchBase);

//============================================================================
/**
 * @function  UsbfnDwc3GetChargerPortType
 *
 * @brief Get Charger Type for the port connected
 *
 *
 * @param IN  CoreNum
 * @param OUT ChargerPortType
 *
 * @return EFI_STATUS
 */
//============================================================================
EFI_STATUS
UsbfnDwc3GetChargerPortType(
  IN  QCOM_USB_CORE_NUM     CoreNum,
  OUT USB_CHARGER_PORT_TYPE *ChargerPortType);
#endif // USB_FN_DWC3_LIB_H
