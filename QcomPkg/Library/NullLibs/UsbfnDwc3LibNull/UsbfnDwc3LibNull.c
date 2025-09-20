//============================================================================
/**
 * @file        UsbfnDwc3Lib.c
 * @author      kameya
 * @date        15-Feb-2013
 *
 * @brief       Platform library for the Synopsys Usbfn driver (UsbfnDwc3)
 *
 * @details     An instance of this library provides platform specific functionallity
 *              for the UsbfnDwc3 driver in order for it to remain platform independed.
 *              This is currently only used for charger detection.
 *
 * @ref         "DWC_usb3_databook 2.5(a)" and "HwProgrammingGuide - usb30 and SS PHY".
 *
 */
//============================================================================
//============================================================================
/**
  Copyright (c) 2015-2018 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================
// ===========================================================================
//
//                            EDIT HISTORY FOR FILE
//   This section contains comments describing changes made to the module.
//   Notice that changes are listed in reverse chronological order.
//
//
// when          who     what, where, why
// ----------   -----    ----------------------------------------------------------
// 05/09/2015   plc      Initial Revision 
// ===========================================================================

#include <Library/UsbfnDwc3Lib.h>
#include "UsbfnDwc3Private.h"
//#include "HalusbHWIO.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIAdc.h>



//----------------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Static Variable Definitions
//----------------------------------------------------------------------------

EFI_STATUS
UsbfnDwc3GetChargerPortType(
  IN  UINT32 CoreId, 
  OUT USB_CHARGER_PORT_TYPE *ChargerPortType) 
{
	return EFI_SUCCESS;
}

//----------------------------------------------------------------------------
// Externalized Function Definitions
//----------------------------------------------------------------------------
//============================================================================
/**
 * @function  dci_connect
 *
 * @brief Connect the device to the USB bus.
 *
 * @Note :  Connect/Disconnect is done via PMIC
 *          It can be done via the USB core when using PIPE3 PHY.
 *
 * @param Core Qscratch Base Address
 *
 * @return none
 *
 * @ref USB 3.0 Controller HPG:
 *      Chapter 4.4.2.8 - ID and VBUS override 
 *
 */
//============================================================================
void dci_connect(QCOM_USB_CORE_NUM  CoreNum,
                 UINTN              QscratchBase)
{
  return;
}

//============================================================================
/**
 * @function  dci_disconnect
 *
 * @brief Disconnect the device from the USB bus.
 *
 * @Note :  Connect/Disconnect is done via PMIC
 *          It can be done via the USB core when using PIPE3 PHY.
 *
 * @param Core Base Address
 *
 * @return none
 *
 * @ref USB 3.0 Controller HPG:
 *      Chapter 4.4.2.8 - ID and VBUS override 

 *
 */
//============================================================================
void dci_disconnect(  
  QCOM_USB_CORE_NUM  CoreNum,
  UINTN              QscratchBase)
{
  return;
}

