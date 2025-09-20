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
 * @ref         "DWC_usb3_databook 3.0(a)" and "HwProgrammingGuide - usb30 and SS PHY".
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
// when         who      what, where, why
// ----------   -----    ----------------------------------------------------------
// 02/13/17`	gk       Port changes from boot.xf.2.0 and boot.xf.2.2
// 11/13/17     pm       Added support for device mode on secondary port
// 10/20/17     gk       Updated new Pmic API for UsbPwrCtrlDxe for UEFI
// 04/04/17     pm       Removed Explicit Addressing from HalusbHWIO header file   
// 12/20/16     sm       Edited input parameter list for ChargerPort PmicUsbProtocol API
// 09/22/16     amitg    Napali Pre-Si Updates
// 02/23/16     ck       deprecate dci_dwc_detect_charger_type and add UsbfnDwc3GetChargerPortType
// 10/26/15     ck       Init 8998 Pre-Si support
// ===========================================================================

#include <Library/UsbfnDwc3Lib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "UsbfnDwc3Private.h"
#include "HalusbHWIO.h"
#include <Protocol/EFIUsbPwrCtrl.h>

//----------------------------------------------------------------------------
// Preprocessor Definitions and Constants
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Externalized Function Definitions
//----------------------------------------------------------------------------

CHAR8* dci_dwc_print_charger_type(dci_dwc_chg_port_type charger_type) {

    switch(charger_type) {
        case DCI_DWC_CHG_PORT_SDP:
            return "Standard Downstream Port (SDP)";
        case DCI_DWC_CHG_PORT_CDP:
            return "Charging Downstream Port (CDP)";
        case DCI_DWC_CHG_PORT_DCP:
            return "Dedicated Charging Port (DCP)";
        case DCI_DWC_CHG_PORT_INVALID:
            return "Invalid charger";
        case DCI_DWC_CHG_PORT_UNKNOWN:
        default:
            return "Unknown charger";
    }

}

//----------------------------------------------------------------------------
// Static Function Declarations and Definitions
//----------------------------------------------------------------------------

//============================================================================
/**
 * @function  dci_enable_external_vbus_config
 *
 * Enables external VBUS configuration. This in turn manually drives D+ pull-up.
 *
 *
 * @param CoreNum
 * @param Core Qscratch Base Address
 *
 *
 *
 * @return none
 */
//=============================================================================
static void
dci_enable_external_vbus_config(
  QCOM_USB_CORE_NUM  CoreNum,
  UINTN              QscratchBase)
{
    /* Unused parameters */
	(void)QscratchBase;
  
 /*
    *Reference USB 3.0 HPG (Oct, 2014)  Default for Primary Core
    */
  if (CoreNum == USB_CORE_0) { //For Primary Core
    //1.    Software updates HS_PHY_CTRL.UTMI_OTG_VBUS_VALID. 
    HWIO_USB3_PRI_HS_PHY_CTRL_OUTM(HWIO_USB3_PRI_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_BMSK, (0x1 << HWIO_USB3_PRI_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_SHFT));
    
    //2.    Software updates HS_PHY_CTRL.SW_SESSVLD_SEL to 1 indicating that the value is driven by UTMI_OTG_VBUS_VALID.
    HWIO_USB3_PRI_HS_PHY_CTRL_OUTM(HWIO_USB3_PRI_HS_PHY_CTRL_SW_SESSVLD_SEL_BMSK, (0x1 << HWIO_USB3_PRI_HS_PHY_CTRL_SW_SESSVLD_SEL_SHFT));
    
    //3.    Software set lane0_power_present
    HWIO_USB3_PRI_SS_PHY_CTRL_OUTM(HWIO_USB3_PRI_SS_PHY_CTRL_LANE0_PWR_PRESENT_BMSK, (0x1 << HWIO_USB3_PRI_SS_PHY_CTRL_LANE0_PWR_PRESENT_SHFT)); 
}
  else if (CoreNum == USB_CORE_1) { //For Secodanry Core
   //1.    Software updates HS_PHY_CTRL.UTMI_OTG_VBUS_VALID. 
    HWIO_USB3_2ND_HS_PHY_CTRL_OUTM(HWIO_USB3_2ND_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_BMSK, (0x1 << HWIO_USB3_2ND_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_SHFT));
    
    //2.    Software updates HS_PHY_CTRL.SW_SESSVLD_SEL to 1 indicating that the value is driven by UTMI_OTG_VBUS_VALID.
    HWIO_USB3_2ND_HS_PHY_CTRL_OUTM(HWIO_USB3_2ND_HS_PHY_CTRL_SW_SESSVLD_SEL_BMSK, (0x1 << HWIO_USB3_2ND_HS_PHY_CTRL_SW_SESSVLD_SEL_SHFT));
    
    //3.    Software set lane0_power_present
    HWIO_USB3_2ND_SS_PHY_CTRL_OUTM(HWIO_USB3_2ND_SS_PHY_CTRL_LANE0_PWR_PRESENT_BMSK, (0x1 << HWIO_USB3_2ND_SS_PHY_CTRL_LANE0_PWR_PRESENT_SHFT)); 
  }
}


//============================================================================
/**
 * @function  dci_disable_external_vbus_config
 *
 * Disables external VBUS configuration. This in turn manually drives D+ pull-down.
 *
 *
 * @param CoreNum
 * @param Core Qscratch Base Address
 *
 *
 *
 * @return none
 */
//=============================================================================
static void
dci_disable_external_vbus_config(
  QCOM_USB_CORE_NUM  CoreNum, 
  UINTN              QscratchBase)
{
    /* Unused parameters */
	(void)QscratchBase;
	
 /*
  *Reference USB 3.0 HPG (Oct, 2014)
    */
  if (CoreNum == USB_CORE_0) {//For Primary Core
    //1.    Software updates HS_PHY_CTRL.UTMI_OTG_VBUS_VALID. 
    HWIO_USB3_PRI_HS_PHY_CTRL_OUTM(HWIO_USB3_PRI_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_BMSK, (0x0 << HWIO_USB3_PRI_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_SHFT));
    
    //2.    Software updates HS_PHY_CTRL.SW_SESSVLD_SEL to 1 indicating that the value is driven by UTMI_OTG_VBUS_VALID.
    HWIO_USB3_PRI_HS_PHY_CTRL_OUTM(HWIO_USB3_PRI_HS_PHY_CTRL_SW_SESSVLD_SEL_BMSK, (0x0 << HWIO_USB3_PRI_HS_PHY_CTRL_SW_SESSVLD_SEL_SHFT));
    
    //3.    Software clear lane0_power_present
    HWIO_USB3_PRI_SS_PHY_CTRL_OUTM(HWIO_USB3_PRI_SS_PHY_CTRL_LANE0_PWR_PRESENT_BMSK, (0x0 << HWIO_USB3_PRI_SS_PHY_CTRL_LANE0_PWR_PRESENT_SHFT)); 
}
  else if (CoreNum == USB_CORE_1) { //For Secodanry Core
   //1.    Software updates HS_PHY_CTRL.UTMI_OTG_VBUS_VALID. 
    HWIO_USB3_2ND_HS_PHY_CTRL_OUTM(HWIO_USB3_2ND_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_BMSK, (0x0 << HWIO_USB3_2ND_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID_SHFT));
    
    //2.    Software updates HS_PHY_CTRL.SW_SESSVLD_SEL to 1 indicating that the value is driven by UTMI_OTG_VBUS_VALID.
    HWIO_USB3_2ND_HS_PHY_CTRL_OUTM(HWIO_USB3_2ND_HS_PHY_CTRL_SW_SESSVLD_SEL_BMSK, (0x0 << HWIO_USB3_2ND_HS_PHY_CTRL_SW_SESSVLD_SEL_SHFT));
    
    //3.    Software set lane0_power_present
    HWIO_USB3_2ND_SS_PHY_CTRL_OUTM(HWIO_USB3_2ND_SS_PHY_CTRL_LANE0_PWR_PRESENT_BMSK, (0x0 << HWIO_USB3_2ND_SS_PHY_CTRL_LANE0_PWR_PRESENT_SHFT)); 
  }
}


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
 * @param Core Qscratch Base Address
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
  UINTN              QscratchBase)
{
    dci_enable_external_vbus_config(CoreNum, QscratchBase);
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
 * @param CoreNum 
 * @param Core Base Address
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
  QCOM_USB_CORE_NUM   CoreNum,
  UINTN               QscratchBase)
{
    dci_disable_external_vbus_config(CoreNum, QscratchBase);
}


EFI_STATUS
UsbfnDwc3GetChargerPortType(
  IN  QCOM_USB_CORE_NUM     CoreNum, 
  OUT USB_CHARGER_PORT_TYPE *ChargerPortType) 
{

  EFI_STATUS Status = EFI_SUCCESS;
  static EFI_USB_PWR_CTRL_PROTOCOL  *PwrCtrlProtocol = NULL;
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE ChgPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID;
  
	
  if (NULL == PwrCtrlProtocol){
    // Find the Power Control USB charger protocol
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&PwrCtrlProtocol);
    if (EFI_ERROR(Status))
    {
      PwrCtrlProtocol = NULL;
      DEBUG((EFI_D_ERROR, "Failed to open Power Control USB protocol Status =  (0x%x)\r\n", Status));
      goto ON_EXIT;
    }
  }

  Status = PwrCtrlProtocol->GetHsUsbChgPortType(CoreNum, &ChgPortType);
  
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "Failed to get ChargerPort Status =  (0x%x)\r\n", Status));
    goto ON_EXIT;
  }

  switch (ChgPortType)
  {
  case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP:
    *ChargerPortType = DCI_DWC_CHG_PORT_CDP;
    break;
  case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP:
    *ChargerPortType = DCI_DWC_CHG_PORT_DCP;
    break;
  case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP:
    *ChargerPortType = DCI_DWC_CHG_PORT_SDP;
    break;
  case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID:
    *ChargerPortType = DCI_DWC_CHG_PORT_UNKNOWN;
    break;
  case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_OCP:
    *ChargerPortType = DCI_DWC_CHG_PORT_INVALID;
    break;
  default:
    *ChargerPortType = DCI_DWC_CHG_PORT_UNKNOWN;
  }

ON_EXIT:
  return Status;
}

