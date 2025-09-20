/** @file \UsbConfigLib.c
 * The file implemented the target specific USB configuration APIs used
 * by the UsbConfigDxe driver.
 */
//============================================================================
/**
  Copyright (c) 2015-2019, 2022 Qualcomm Technologies, Inc. All rights reserved.
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
 04/18/19   gk       Fix the UsbConfigDxe Taking More Boot time
 03/26/19   ak       Support for advancing TX SuperSpeed compliance test pattern
 02/21/19   gk       Added Support for LA Flavor for MultiPort
 01/16/19   gk       Added Support to Turnoff NPA for all cores.
 11/13/18   gk       Added support for LPM in UEFI
 10/22/18   kameya   Added code for new PwrctrlDxe Api
 10/11/18   kameya   Support USB SLT loopback feature.
 05/03/18   pm       Fix UsbConfigInit issue for CLS Platform 
 04/19/18   amitg    Poipu Multi-Port Support, Vbus fixes
 04/10/18   amitg    Poipu BU Fixes
 04/06/18   amitg    Enabled Dual Role for Primary and Secondary Port
 04/03/18   psapin   For Poipu BU: primary to Host mode, add StartController for prim, other ConfigInit changes
 02/13/18   gk       Port changes from boot.xf.2.0 and boot.xf.2.2
 01/30/18   pm       Fixed issue to StartUsbController, if no USB cable connected during bootup 
 01/22/18   gk       Added USB Variable to Enable High Speed Mode For Display port
 11/13/17   pm       Added support for device mode on secondary port
 11/14/17   gk       Enabling Device Mode on CLS_PlatForm
 10/20/17   gk       Updated new Pmic API for UsbPwrCtrlDxe for UEFI
 11/27/17   amitg    MP Core Changes for Poipu
 09/27/17   vsb      Add ACPI patching for QcUsbFN HLOS
 09/04/17   gk       UEFI USB compliance test changes
 08/28/17   psapin   Update/Fix SMMU Stage 1 Support for WP 
 08/10/17   amitg    Init Changes for Poipu
 07/31/17   psapin   Add SMMU Stage 1 Support //need to port SMMU changes or not *Gautam
 07/19/17   amitg    Updated Core Count
 06/14/17   amitg    Cleanup
 04/26/17   amitg    Remove Power down sequence of primary core in Exit Boot Services
 04/19/17   amitg    Sec Core Updates, Primary Core enable pmic call
 03/22/17   pm       Add Sec core, Remove UsbCSetPortRole
 03/21/17   amitg    SDM845 UEFI USB Updates
 03/06/17   amitg    EUD Support
 02/03/17   pm       Vbus Enablement in USB Config Protocol
 02/02/17   pm       Port from Boot.xf.1.4
 01/19/17   pm       Fix KW issue
 05/13/16   ck       honor static configuration in stop controller
 05/06/16   ck       Temp workaround by enable VBUS asap
 04/12/16   ck       Add the support to USB init protocol
 03/31/16   ck       Fix incorrect param check
 02/23/16   ck       Isolate USB Core and Port data to its own module
 01/22/16   ck       Support on demand USBFn configuration
 01/14/16   ck       Fix incorrect call to enter lpm in exit UEFI service
 10/26/15   ck       Init 8998 Pre-Si support
=============================================================================*/

//
// Includes
//
#include "UsbConfigLibPrivate.h"
#include <Library/SerialPortShLib.h>

#include <Protocol/EFIAcpiPlatform.h>
#include <Protocol/EFIChipInfoTypes.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/EFIChargerEx.h>
#include <IndustryStandard/Acpi.h>

#define EFI_DEVICE_USB_QC_COMPOSITE     "QUFN"      //ACPI feature name for QC composite
#define USB_VAR_QCFN_HLOS               L"UsbfnEnableQcFnHLOS"
#define MAX_VARIABLE_NAME_SIZE          50

extern EFI_PLATFORMINFO_PLATFORM_INFO_TYPE    PlatformInfo;
extern EFIChipInfoVersionType                 chipVersion;

EFI_HAL_IOMMU_PROTOCOL        *gHalIommuProtocol = NULL;
HAL_IOMMU_fptable_t           *gSmmuTable = NULL;
void                          *gUsbSmmuDomain = NULL;
BOOLEAN                       gSmmuIsSetup = FALSE;
BOOLEAN                       gSmmuUsb30Attached = FALSE;
BOOLEAN                       gSmmuUsb31Attached = FALSE;
BOOLEAN                       gSmmuUsb32Attached = FALSE;
BOOLEAN                       gHighSpeedOnlyPrimary = FALSE;
BOOLEAN                       gHighSpeedOnlySecondary = FALSE;
BOOLEAN                       gUSBPriDPBonded = FALSE;
BOOLEAN                       gUSBSecDPBonded = FALSE;

//UEFI Variable Name and its default value for each core 
CHAR16                     
gVariableName[USB_CORE_MAX_NUM_SDM_1000][MAX_VARIABLE_NAME_SIZE] = {L"UsbConfigPrimaryPort", 
                                                                   L"UsbConfigSecondaryPort"
                                                                   L"UsbHostMultiPort"};

UINT8 
gDefaultVariableVal[USB_CORE_MAX_NUM_SDM_1000] =  {USB_DUAL_ROLE_SDM_1000, 
                                                  USB_DUAL_ROLE_SDM_1000,
                                                  USB_HOST_MODE_SDM_1000}; 
//Variable to set Static Configuration based on first time cable connected
BOOLEAN 
gDualRoleEnable[USB_CORE_MAX_NUM_SDM_1000]     =  {FALSE, FALSE, FALSE};

static EFI_ACPI_SDT_PROTOCOL      *gAcpiSdt = NULL;
EFI_EVENT AcpiSdtProtocolNotificationEvent  = (EFI_EVENT)NULL;
VOID      *AcpiSdtProtocolNotificationToken = NULL;
EFI_USB_PWR_CTRL_USB_PORTHW_INFO port_hw_info;
BOOLEAN   gOffModeCharging = FALSE;

//
// For each USB controller in the system, an USB_CONFIG_DEVICE is created to 
// hold the controller handles and configuration data associated with the
// current USB configuration.
// A dummy core is always appended at the end of the device array.
//
static USB_CONFIG_DEVICE gUsbConfigDevice[] = {
  { USB_CONFIG_DEV_SIGNATURE,     //signature
    NULL,                         //host handle
    NULL,                         //peripheral handle
    NULL,                         //QCOM_USB_CORE_INTERFACE Primary 
    NULL,                         //QCOM_USB_PORT handle Primary
    {0}                           //QCOM_USB_CONFIG_PROTOCOL
  }, // Primary
  { USB_CONFIG_DEV_SIGNATURE,     //signature
    NULL,                         //host handle
    NULL,                         //peripheral handle
    NULL,                         //QCOM_USB_CORE_INTERFACE Secondary 
    NULL,                         //QCOM_USB_PORT handle Secondary
    {0}                           //QCOM_USB_CONFIG_PROTOCOL
  }, // secondray
  { USB_CONFIG_DEV_SIGNATURE,     //signature
    NULL,                         //host handle
    NULL,                         //peripheral handle
    NULL,                         //QCOM_USB_CORE_INTERFACE dummy
    NULL,                         //QCOM_USB_PORT handle dummy
    {0}                           //QCOM_USB_CONFIG_PROTOCOL
  }, // MP Core
  { USB_CONFIG_DEV_SIGNATURE,     //signature
    NULL,                         //host handle
    NULL,                         //peripheral handle
    NULL,                         //QCOM_USB_CORE_INTERFACE dummy
    NULL,                         //QCOM_USB_PORT handle dummy
    {0}                           //QCOM_USB_CONFIG_PROTOCOL
  } //Dummy used in case no valid cores are initialized
};

/**
  Get the charger port type

  @param [in] CoreNum            Core number
  @param [Out] ChargerPortType    Charger port type

  @retval EFI_SUCCESS            Charger port type is retrieved correct
  @retval Others                 Operation failed.
**/
EFI_STATUS
GetChargerPortType(
  IN  QCOM_USB_CORE_NUM CoreNum,
  OUT EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *ChargerPortType)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8   UefiVar = 0;
  UINTN   UefiVarSize;
  
  // Check if the UEFI variable is set to enable device mode compliance testing
  // Override charger port type to SDP for compliance testing
  if(CoreNum == USB_CORE_0)
  {
    UefiVarSize = sizeof(UefiVar);
    Status = gRT->GetVariable(L"UsbfnForceComplianceMode", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
    if (!EFI_ERROR(Status) && (UefiVar != 0)) {
      DBG(EFI_D_WARN, "USB Device mode compliance override set");
      *ChargerPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP;
      goto ON_EXIT;
    }
  }

  //Assume ChargerPortType to SDP for Secondary Port 
  //as there is no support to get ChargerType from PMIC for Secondary Port - Hardcoded
  if(CoreNum == USB_CORE_1)
  {
    Status = EFI_SUCCESS;
    DBG(EFI_D_WARN, "Detect Secondary Port, forced to SDP");
    *ChargerPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP;
    goto ON_EXIT;
  }

  *ChargerPortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID;

  // Get the Power Control USB charger protocol if not already available.
  if (NULL == gPwrCtrlProtocol) {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
    if (EFI_ERROR(Status))
    {
      gPwrCtrlProtocol = NULL;
      DBG(EFI_D_ERROR, "Failed to open Power Control USB protocol Status =  (0x%x)", Status);
      goto ON_EXIT;
    }
  }

  // ChargerPort function now guarantees that BC 1.2 charger detection finishes
  // before the function returns.
  //CoreNum maps to the port index.
  Status = gPwrCtrlProtocol->GetHsUsbChgPortType(CoreNum, ChargerPortType);
  if (EFI_ERROR(Status))
  {
    DBG(EFI_D_ERROR, "Failed to call PMIC ChargerPort Status =  (0x%x)", Status);
    goto ON_EXIT;
  }

ON_EXIT:
  return Status;
}


/**
  Returns core number for a given core type

  @param [in]  CoreType           USB core type.
  @param [out] CoreNum            Core number

  @retval EFI_SUCCESS             CoreNum is valid
  @retval EFI_INVALID_PARAMETER   CoreNum is NULL
  @retval EFI_UNSUPPORTED         Unsupported CoreType.
**/
EFI_STATUS
EFIAPI
GetCoreNum (
  IN  QCOM_USB_CONFIG_CORE_TYPE   CoreType,
  OUT QCOM_USB_CORE_NUM          *CoreNum
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  if (NULL == CoreNum) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("GetCoreNum: Error - CoreNum is NULL");
  }

  switch (CoreType)
  {
    case USB_CONFIG_SSUSB1:
      *CoreNum = USB_CORE_0;
      break;
    case USB_CONFIG_SSUSB2:
      *CoreNum = USB_CORE_1;
      break;  
    case USB_CONFIG_SSUSB3:
       *CoreNum = USB_CORE_2;
        break;
    default:
      DBG(EFI_D_ERROR, "GetCoreNum: Error - unsupported USB core type 0x%x", CoreType);
      Status = EFI_UNSUPPORTED;
      *CoreNum = (QCOM_USB_CORE_NUM) USB_CORE_MAX_NUM_SDM_1000;
    break;
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Returns base address of the specified USB core.

  @param [in]  CoreType           USB core type.
  @param [out] BaseAddr           Register set base address output.

  @retval EFI_SUCCESS             BaseAddr is valid
  @retval EFI_INVALID_PARAMETER   BaseAddr is NULL
  @retval EFI_UNSUPPORTED         Unsupported CoreType.
**/
EFI_STATUS
EFIAPI
GetCoreBaseAddr (
  IN  QCOM_USB_CONFIG_CORE_TYPE   CoreType,
  OUT UINTN                       *BaseAddr
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE_NUM   CoreNum;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;

  FNC_ENTER_MSG ();

  if (NULL == BaseAddr) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("GetCoreBaseAddr: Error - BaseAddr is NULL");
  }

  if (CoreType >= USB_CONFIG_CORE_MAX) {
    Status = EFI_UNSUPPORTED;
    ERR_CHK("GetCoreBaseAddr: Error - unsupported USB core type 0x%x",CoreType);
  }

  Status = GetCoreNum(CoreType, &CoreNum);
  ERR_CHK ("failed to get USB core number");
  if (CoreNum >= (QCOM_USB_CORE_NUM)USB_CORE_MAX_NUM_SDM_1000) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("CoreNum out of range");
  }
  
  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;
  Status = UsbCoreIfc->GetBaseAddress(UsbCoreIfc, BaseAddr);
  ERR_CHK("UsbCoreIfc->GetBaseAddress failed with status %r", Status);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Initializes the specified USB core for in host or device mode.

  @param [in] Mode              Host or device mode
  @param [in] CoreNum           Index of core supporting Mode to initialize

  @retval EFI_SUCCESS           USB initialized successfully.
  @retval EFI_UNSUPPORTED       Unsupported ConfigType.
**/
EFI_STATUS
EFIAPI
ConfigUsb (
  IN QCOM_USB_MODE_TYPE         Mode,
  IN QCOM_USB_CORE_NUM          CoreNum
  )
{
  EFI_STATUS                  Status   = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE     *UsbCoreIfc;

  FNC_ENTER_MSG ();

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("ConfigUsb: Error - Invalid CoreNum passed: %d", CoreNum);
  }

  if ((CoreNum == USB_CORE_2_SDM_1000) && (Mode != USB_HOST_MODE_XHCI))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("ConfigUsb: Error - Invalid CoreNum %d doesnt support device mode", CoreNum);
  }
  DBG(EFI_D_INFO,"CoreNum %d, Mode %d", CoreNum, Mode);

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  USB_ASSERT_RETURN ((NULL != UsbCoreIfc), EFI_DEVICE_ERROR);
  
  // initialize the core
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc->InitCommon), EFI_DEVICE_ERROR);
  Status = UsbCoreIfc->InitCommon(UsbCoreIfc, FALSE);
  ERR_CHK ("ConfigUsb: Error - failed to perform common initialization for USB core %d: %r",
      CoreNum, Status);

  if (Mode == USB_DEVICE_MODE_SS) {
    USB_ASSERT_RETURN ((NULL != UsbCoreIfc->InitDevice), EFI_DEVICE_ERROR);
    Status = UsbCoreIfc->InitDevice(UsbCoreIfc);
    ERR_CHK ("ConfigUsb: Error - failed to perform device initializaition for USB core %d: %r",
        CoreNum, Status);
  } else {
    USB_ASSERT_RETURN ((NULL != UsbCoreIfc->InitHost), EFI_DEVICE_ERROR);
    Status = UsbCoreIfc->InitHost(UsbCoreIfc);
    ERR_CHK ("ConfigUsb: Error - failed to perform host initializaition for USB core %d: %r",
        CoreNum, Status);
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Resets the USB core and reinitializes in the appropriate mode (host or device).

  @param [in] ConfigType        Configuration type
  @param [in] DeviceMode        Reinitialize in host or device mode

  @retval EFI_SUCCESS           USB reset successfully
  @retval Others                Error resetting core

**/
EFI_STATUS
EFIAPI
ResetUsb (
  IN QCOM_USB_CONFIG_CORE_TYPE  CoreType,
  IN BOOLEAN                    DeviceMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
  QCOM_USB_CORE_NUM CoreNum;

  FNC_ENTER_MSG ();

  if (CoreType >= USB_CONFIG_CORE_MAX) {
    Status = EFI_UNSUPPORTED;
    ERR_CHK("ResetUsb: Error - unsupported USB core 0x%x", CoreType);
  }

  Status = GetCoreNum(CoreType, &CoreNum);
  ERR_CHK ("ResetUsb: Error - failed to get USB core number");
  if (CoreNum >= (QCOM_USB_CORE_NUM)USB_CORE_MAX_NUM_SDM_1000) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("CoreNum out of range");
  }
  
  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  // reset the core
  USB_ASSERT_GOTO_SETSTS ((NULL != UsbCoreIfc->Reset), ON_EXIT,
                          EFI_DEVICE_ERROR, Status);
  Status = UsbCoreIfc->Reset(UsbCoreIfc);
  ERR_CHK ("ResetUsb: Error - failed to reset USB core %d: %r", CoreType, Status);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Enter low power mode for USB Core

  @param  [in] Mode             Host or device mode
  @param  [in] CoreNum          Index of core to be entered in low power mode

  @retval EFI_SUCCESS           USB core entered LPM successfully.
  @retval EFI_INVALID_PARAMETER Unsupported Core
  @retval Other                 Operation failed
**/

EFI_STATUS
EFIAPI
EnterLPM (
  IN QCOM_USB_MODE_TYPE         Mode,
  IN QCOM_USB_CORE_NUM          CoreNum
  )
{
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("EnterLPM: Error - Invalid core passed: %d", CoreNum);
  }

  Status = UsbCoreIfc->CoreEnterLPM(UsbCoreIfc);
  ERR_CHK ("EnterLPM: Error - failed to enter low power mode for USB coreNum %d: %r", CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Exits low power mode for USB Core

  @param [in] Mode                    Host or device mode
  @param [in] CoreNum                 Index of core supporting Mode to initialize

  @retval EFI_SUCCESS                 USB core exited LPM successfully.
  @retval EFI_INVALID_PARAMETER       Unsupported Core
  @retval Other                       Operation failed
**/

EFI_STATUS
EFIAPI
ExitLPM (
  IN QCOM_USB_MODE_TYPE         Mode,
  IN QCOM_USB_CORE_NUM          CoreNum
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
 
  FNC_ENTER_MSG ();

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("ExitLPM: Error - Invalid core passed: %d", CoreNum);
  }

  Status = UsbCoreIfc->CoreExitLPM(UsbCoreIfc);
  ERR_CHK ("ExitLPM: Error - failed to exit low power mode for USB coreNum %d: %r", CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Return appropriate core type describing the ChipIdea core to be used in
  device mode. The Synopsys Usbfn driver will use a separate function.

  @param [out]  CoreType         Configuration type.

  @retval EFI_SUCCESS            Success
  @retval EFI_UNSUPPORTED        Unsupported ConfigType.

**/
EFI_STATUS
EFIAPI
GetUsbFnConfig (
  OUT QCOM_USB_CONFIG_CORE_TYPE *CoreType
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Return appropriate core type for each platform

  @param [in]  CoreNum           Core number supporting Mode
  @param [out] CoreType          Configuration type.

  @retval EFI_SUCCESS            Success
  @retval EFI_UNSUPPORTED        Unsupported ConfigType.

**/
EFI_STATUS
EFIAPI
GetSSUsbFnConfig (
  IN  QCOM_USB_CORE_NUM         CoreNum,
  OUT QCOM_USB_CONFIG_CORE_TYPE *CoreType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();
  
  if (NULL == CoreType) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("GetSSUsbFnConfig: Error - CoreType is NULL");
  }

  switch(CoreNum) {
  case USB_CORE_0: 
    *CoreType = USB_CONFIG_SSUSB1;
    break;
  case USB_CORE_1:
    *CoreType = USB_CONFIG_SSUSB2;
    break;
  case USB_CORE_2:
    *CoreType = USB_CONFIG_SSUSB3;
    break;
  default: 
    Status = EFI_INVALID_PARAMETER;
    *CoreType = USB_CONFIG_SSUSB1;
    ERR_CHK("GetUsbHostConfig: Error - CoreNum is %d, but expect < %d", CoreNum, USB_CORE_MAX_NUM_SDM_1000);
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Return appropriate core type to use for the specified host mode and index.

  @param [in]  Mode             USB_HOST_MODE_EHCI or USB_HOST_MODE_XHCI
  @param [in]  CoreNum          Core number supporting Mode
  @param [out] CoreType         Core type fitting above parameters

  @retval EFI_SUCCESS           CoreType contains a valid core
  @retval EFI_UNSUPPORTED       Unsupported combination of parameters

**/
EFI_STATUS
EFIAPI
GetUsbHostConfig (
  IN  QCOM_USB_MODE_TYPE        Mode,
  IN  QCOM_USB_CORE_NUM         CoreNum,
  OUT QCOM_USB_CONFIG_CORE_TYPE *CoreType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();
  
  if (NULL == CoreType) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("GetUsbHostConfig: Error - CoreType is NULL");
  }

  switch(CoreNum) {
    case USB_CORE_0_SDM_1000:
        *CoreType = USB_CONFIG_SSUSB1;
        break;
    case USB_CORE_1_SDM_1000:
      *CoreType = USB_CONFIG_SSUSB2;
      break;
    case USB_CORE_2_SDM_1000:
      *CoreType = USB_CONFIG_SSUSB3;
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
      *CoreType = USB_CONFIG_CORE_MAX;
      ERR_CHK("GetUsbHostConfig: Error - CoreNum is %d, but expect USB_CORE_0/1/2", CoreNum);
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}

/**
  UsbConfigInit does HW/driver initialization that's not dependent on other driver requests.

  @retval EFI_SUCCESS            Success.
  @retval EFI_DEVICE_ERROR       Failure

**/
EFI_STATUS
EFIAPI
UsbConfigInit (
  VOID
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  INT8                          UefiVar = 0;
  UINTN                         UefiVarSize;
  UINTN                         Index;
  HAL_IOMMUResult_t             SmmuStatus = HAL_IOMMU_ERR_OK;
  EFI_CHARGER_EX_PROTOCOL       *pChargerExProtocol;
  
  FNC_ENTER_MSG ();
  
  // Check if there's already a stack running on the core. 
  // If InitUsbControllerOnBoot is one, this should not happen, and we should log the error message.
  // If InitUsbControllerOnBoot is zero, this may happen, and it depends on the client application implementation. 
  if (gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].PeripheralHandle != NULL)
  {
    UINT32 PrintFlag = EFI_D_INFO;
    if (FeaturePcdGet(InitUsbControllerOnBoot)) {
      PrintFlag = EFI_D_ERROR;
    }
    Status = EFI_SUCCESS;
    DBG(PrintFlag, "USBConfigInit called more than once");
    goto ON_EXIT;
  }

  // Since almost all the USB resources are shared with other drivers. 
  //This Function only able to close LDO16 and rest are still ON. So, disabling this function for boot time optimization.
  
  /*Turn off NPA resources for all the Usb Cores.
  Status = UsbConfigLibCloseNPA();
  if(Status != EFI_SUCCESS){
      DBG(EFI_D_INFO, "Failed to Disable clocks! Status =  (0x%x)", Status);
    }*/
#ifdef ENABLE_AUTO_PLAT
  if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_ADP)&&(PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_OEM))
#else
  if (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_ADP)
#endif
  {
  Status = gBS->LocateProtocol(
            &gChargerExProtocolGuid, 
            NULL,
            (VOID **) &pChargerExProtocol);
    
    if ((Status != EFI_SUCCESS) || (NULL == pChargerExProtocol)) 
    {
       DEBUG((EFI_D_WARN,"ERROR: ChargerTest - Failed to locate Charger Protocol! Error Code = 0x%08X\r\n", Status));
       goto ON_EXIT;
    }
    /*
    * Check if the device is entering off mode charging
    *
    * TRUE  - it is in offmode charging, Enter Low Power Mode
    * FALSE - continue with normal init operation
    * ChargExProtocol doesn't handle RUMI and CDP platform 
    * So, Ignoring both Platform's Status.
    */
    Status = pChargerExProtocol->IsOffModeCharging(&gOffModeCharging);
     if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI) && 
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_CDP) && 
      (Status != EFI_SUCCESS))
     {
       DEBUG((EFI_D_WARN,"ERROR: ChargerTest - Failed to get OffModeCharging Status! Error Code = 0x%08X\r\n", Status));
       goto ON_EXIT;
    } 
      
    
    if (gOffModeCharging){
      DEBUG((EFI_D_WARN,"%a: Aborting USB Initialization due to dead battery: \r\n", __func__));
      goto ON_EXIT;
    }

 }
  // Allocate a handle w/ invalid core config to be used by clients to start a controller in case where no valid USB controller handles exist
  gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].UsbConfig = gUsbConfigTemplate;
  gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].UsbConfig.coreNum = (QCOM_USB_CORE_NUM)USB_CORE_MAX_NUM_SDM_1000;
  gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].UsbConfig.modeType = USB_INVALID_MODE;
  gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].HostHandle = NULL;
  gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].PeripheralHandle = NULL;

  //Check if the UEFI variable is set to enable HighSpeedOnlySupportedForDP in UEFI. If so, 
  //enable gHighSpeedOnlyPrimary
  UefiVarSize = sizeof(UefiVar);
  Status = gRT->GetVariable(L"EnableHighSpeedOnlyForPri", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
  if(!EFI_ERROR(Status)) {
    DBG(EFI_D_WARN, "USB force High Speed Only mode for Primary port via Menu HSOnly=%d", UefiVar);
    if (UefiVar != 0) {
      gHighSpeedOnlyPrimary = TRUE;
    }
  } else if (FeaturePcdGet(USBPriHighSpeedOnlySupported)) {
    DBG(EFI_D_WARN, "USB force High Speed only mode set for Primary port via PCD");
    gHighSpeedOnlyPrimary = TRUE;
  }

  //Check if the UEFI variable is set to enable USBSecHighSpeedOnlySupported in UEFI. If so, 
  //enable gHighSpeedOnlySecondary
  UefiVar = 0;
  UefiVarSize = sizeof(UefiVar);
  Status = gRT->GetVariable(L"EnableHighSpeedOnlyForSec", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
  if(!EFI_ERROR(Status)) {
    DBG(EFI_D_WARN, "USB force High Speed Only mode for Secondary port via Menu HSOnly=%d", UefiVar);
    if (UefiVar != 0) {
      gHighSpeedOnlySecondary = TRUE;
    }
  } else if (FeaturePcdGet(USBSecHighSpeedOnlySupported)) {
    DBG(EFI_D_WARN, "USB force High Speed only mode set for Secondary port via PCD");
    gHighSpeedOnlySecondary = TRUE;
  }

  //Check DPBonded Primary
  UefiVar = 0;
  UefiVarSize = sizeof(UefiVar);
  Status = gRT->GetVariable(L"EnableDPBondedForPri", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
  if(!EFI_ERROR(Status)) {
    DBG(EFI_D_WARN, "Skip GCC Reset for DP Bonded Mode for Primary Port via Menu DPBonded=%d", UefiVar);
    if (UefiVar != 0) {
      gUSBPriDPBonded = TRUE;
    }
  } else if (FeaturePcdGet(USBPriDPBonded)) {
    DBG(EFI_D_WARN, "Skip GCC Reset for DP Bonded Mode for Primary Port via PCD");
    gUSBPriDPBonded = TRUE;
  }

  //Check DPBonded Secondary
  UefiVar = 0;
  UefiVarSize = sizeof(UefiVar);
  Status = gRT->GetVariable(L"EnableDPBondedForSec", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
  if(!EFI_ERROR(Status)) {
    DBG(EFI_D_WARN, "Skip GCC Reset for DP Bonded Mode for Secondary Port via Menu DPBonded=%d", UefiVar);
    if (UefiVar != 0) {
      gUSBSecDPBonded = TRUE;
    }
  } else if (FeaturePcdGet(USBSecDPBonded)) {
    DBG(EFI_D_WARN, "Skip GCC Reset for DP Bonded Mode for Secondary Port via PCD");
    gUSBSecDPBonded = TRUE;
  }


  // Install the protocol on PeripheralHandle, this is used as an indicator that
  // the UsbConfigInit has been called. 
  Status = gBS->InstallMultipleProtocolInterfaces(&gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].PeripheralHandle,
    &gQcomUsbConfigProtocolGuid, &gUsbConfigDevice[USB_CORE_MAX_NUM_SDM_1000].UsbConfig,
    NULL);
  ERR_CHK("UsbConfigInit: Error - Failed to install USB_CONFIG protocol");

  // Initializes UsbConfigDevice for the USB controllers in the system
  for (Index = 0; Index < USB_CORE_MAX_NUM_SDM_1000; Index++) {
    gUsbConfigDevice[Index].UsbConfig = gUsbConfigTemplate;
    gUsbConfigDevice[Index].UsbConfig.coreNum = (QCOM_USB_CORE_NUM)Index;
    gUsbConfigDevice[Index].UsbConfig.modeType = USB_INVALID_MODE;
    gUsbConfigDevice[Index].HostHandle = NULL;
    gUsbConfigDevice[Index].PeripheralHandle = NULL;

    Status = UsbConfigCoresInit(
      gUsbConfigDevice[Index].UsbConfig.coreNum,
      &gUsbConfigDevice[Index].UsbCoreInterface);
    ERR_CHK("UsbConfigUsbCoresInit failed on Core %d, %r", 
      gUsbConfigDevice[Index].UsbConfig.coreNum, 
      Status);

    // NOTE: VOID ** Casting required to resolve KW treating 
    // "&" operator as 8bytes VOID** as 4bytes
    Status = UsbConfigPortsInit(
      gUsbConfigDevice[Index].UsbConfig.coreNum,
      (VOID**)&gUsbConfigDevice[Index].UsbPortHandle);
    ERR_CHK("UsbConfigPortsInit failed on Core %d, %r",
      gUsbConfigDevice[Index].UsbConfig.coreNum,
      Status);
  }


  //Setup SMMU Stage 1
  if (NULL == gHalIommuProtocol) {
    SmmuStatus = gBS->LocateProtocol(&gEfiHalIommuProtocolGuid, NULL, (void **)&gHalIommuProtocol);
    if (EFI_ERROR(SmmuStatus))
    {
      gHalIommuProtocol = NULL;
      DBG(EFI_D_ERROR, "Failed to open HAL IOMMU protocol Status =  (0x%x)", SmmuStatus);
    } else {
      gHalIommuProtocol->_HAL_IOMMU_protocol_init((HAL_IOMMU_fptable_t **)&gSmmuTable);
      SmmuStatus = gSmmuTable->domain_create_func(&gUsbSmmuDomain);
      SMMU_CHK("Failed to create HAL IOMMU domain Result =  (0x%x)", SmmuStatus);
      if (gSmmuIsSetup) {
        SmmuStatus = gSmmuTable->config_bypass_domain_func(gUsbSmmuDomain, HAL_IOMMU_AARCH64_NON_CCA_DOMAIN_TYPE);
        SMMU_CHK("Failed to configure HAL IOMMU usb domain Result  =  (0x%x)", SmmuStatus);
      }
      // attach
      if (gSmmuIsSetup) {
        SmmuStatus = gSmmuTable->domain_attach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-0", 0x0, 0x0);
        SMMU_CHK("Failed to attach USB3-0 HAL IOMMU domain Result =  (0x%x)", SmmuStatus);
        gSmmuUsb30Attached = gSmmuIsSetup;
        SmmuStatus = gSmmuTable->domain_attach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-1", 0x0, 0x0);
        SMMU_CHK("Failed to attach USB3-1 HAL IOMMU domain Result =  (0x%x)", SmmuStatus);
        gSmmuUsb31Attached = gSmmuIsSetup;
        SmmuStatus = gSmmuTable->domain_attach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-2", 0x0, 0x0);
        SMMU_CHK("Failed to attach USB3-2 HAL IOMMU domain Result =  (0x%x)", SmmuStatus);
        gSmmuUsb32Attached = gSmmuIsSetup;
        gSmmuIsSetup = TRUE;
      }
    }   
  }
     
  // Check if we have Clamshell platform
  if (IS_CLS_PLAT(PlatformInfo.platform)) {

    //Set Default mode to host mode on secondary USB port for CLS_PLATFORM
    gDefaultVariableVal[USB_CORE_1_SDM_1000] = USB_HOST_MODE_SDM_1000;
  }
  
  for (Index = 0; Index < USB_CORE_MAX_NUM_SDM_1000; Index++) {
	UefiVar = USB_DUAL_ROLE_SDM_1000;
    UefiVarSize = sizeof(UefiVar);
    Status = gRT->GetVariable(gVariableName[Index], &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);
    if (EFI_ERROR(Status)){
      UefiVar = gDefaultVariableVal[Index];
    }

    switch(UefiVar) { 
      case USB_HOST_MODE_SDM_1000:
        DBG(EFI_D_INFO, "Host Mode override set on Core Number: %d", Index);
        gUsbConfigDevice[Index].EnableStaticCfg.Enabled = TRUE;
        gUsbConfigDevice[Index].EnableStaticCfg.UsbMode = USB_HOST_MODE_XHCI;
        gUsbConfigDevice[Index].UsbCoreInterface->ModeType = USB_HOST_MODE;
        break;

      case USB_DEVICE_MODE_SDM_1000:
        DBG(EFI_D_INFO, "Device Mode override set on Core Number: %d", Index);
        gUsbConfigDevice[Index].EnableStaticCfg.Enabled = TRUE;
        gUsbConfigDevice[Index].EnableStaticCfg.UsbMode = USB_DEVICE_MODE_SS;
        gUsbConfigDevice[Index].UsbCoreInterface->ModeType = USB_DEVICE_MODE;
        break;

      case USB_DUAL_ROLE_SDM_1000:
        // If the user plugs in the host cable during the boot time, configure the 
        // USB hardware in the host mode
        DBG(EFI_D_INFO, "Dual Role Enabled on Core Number: %d", Index);
        gDualRoleEnable[Index] = TRUE;
        break;
        
      default:
        DBG(EFI_D_WARN, "Unable to decide the mode of the connection at core : %d", Index);
        goto ON_EXIT;
        break;
        
    }
    
    if (Index == USB_CORE_0_SDM_1000 && gUsbConfigDevice[Index].UsbCoreInterface->ModeType == USB_HOST_MODE) {
      //Software based Vbus Enablement always
      if(gPwrCtrlProtocol != NULL) {          
        //Enable VBUS for Poipu bring up
        Status = gPwrCtrlProtocol->SetVbusSourceEn(Index, TRUE);
        WRN_CHK("SetVbusSourceEn failed on Port Number: %d, Status: %r", Index, Status);
      }
      Status = UsbStartController((QCOM_USB_CORE_NUM)Index, USB_HOST_MODE_XHCI);
      ERR_CHK("Start controller failed with error: %r on core %d", Status, Index);
    }
    
    //Need to check Vbus enablement for sec core in the next round of changes.
    else if (Index == USB_CORE_1_SDM_1000 && gUsbConfigDevice[Index].UsbCoreInterface->ModeType == USB_HOST_MODE)
    {
      //Start the controller in host mode (Secondary port) - Hardcoded
      //For CLS/MTP - Internally need to start the controller
      //For Hanami -  we may need to remove UsbStartController call as client can request for Host mode Explicity
      Status = UsbStartController((QCOM_USB_CORE_NUM)Index, USB_HOST_MODE_XHCI);
      ERR_CHK("Start controller failed with error: %r on core %d", Status, Index);
    }
    
    //Also Need to check Vbus enablement for multi core in the next round of changes.
    else if (Index == USB_CORE_2_SDM_1000 && gUsbConfigDevice[Index].UsbCoreInterface->ModeType == USB_HOST_MODE && FeaturePcdGet(USBMultiPortSupported))
    {
      // Start MP Controller in host mode always
      DBG(EFI_D_INFO, "Start MP Controller");
      Status = UsbStartController((QCOM_USB_CORE_NUM)Index, USB_HOST_MODE_XHCI);
      ERR_CHK("Start controller failed with error: %r on core %d", Status, USB_CORE_2_SDM_1000);
    }
  }

  //Get the Info about Number of ports active.
  if(gPwrCtrlProtocol != NULL) {
    gPwrCtrlProtocol->GetHwInfo(&port_hw_info);
    DBG(EFI_D_WARN, "UsbPwrCtrl No. of Ports = %d\r\n", port_hw_info.NumOfPorts);
  }

  // Create a timer to poll USB Type-C CC Status
  Status = gBS->CreateEvent(
    EVT_TIMER | EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    UsbPortStateChgCb,
    NULL,
    &PortStateChgEvt
    );
  ERR_CHK("Failed to create USB Type-C Event with error: %r", Status);

  // Signal the event immediately.
  gBS->SignalEvent(PortStateChgEvt);

  // Set the polling period for the USB Type-C periodic event callback
  Status = gBS->SetTimer(
    PortStateChgEvt,
    TimerPeriodic,
    USB_PORT_POLL_INTERVAL
    );
  ERR_CHK("Failed to set polling period for USB Type-C Event with error: %r", Status);
  
  //Check if the UEFI variable is set to enable QcUsbFN in HLOS. If so, 
  //register for ACPI protocol callbacks
  UefiVar = 0;
  UefiVarSize = sizeof(UefiVar);
  
  Status = gRT->GetVariable(USB_VAR_QCFN_HLOS, &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &UefiVar);

  if (!EFI_ERROR(Status) && (UefiVar != 0)) 
  {
    Status = UsbConfigUpdateACPIEntry();
    if (EFI_ERROR (Status)) 
    {
        DEBUG((EFI_D_ERROR, "[UsbConfig]UsbConfigUpdateACPIEntry: Failed to Locate SDT protocol \r\n"));
        Status = EFI_SUCCESS;
        goto ON_EXIT;
     }
  }
  else
  {
    Status = EFI_SUCCESS;
  }
ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/** 
  @brief Perform the clean up tasks for exiting boot service.

  When the UsbConfigExitBootServiceEvent is invoked in the UsbConfigDxe to prepare
  the lib for exiting the boot service.
  Unreleased resources, if any, allocated within the lib should now be freed.

  @retval EFI_SUCCESS            The operation succeeded.
  @retval EFI_UNSUPPORTED        The operation failed.

**/
EFI_STATUS
EFIAPI
UsbConfigLibExitBootService (
  VOID
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;

  UINTN Index;
  
  FNC_ENTER_MSG ();

  if (PortStateChgEvt) {
    gBS->CloseEvent(PortStateChgEvt);
    PortStateChgEvt = NULL;
  }

  if (AcpiSdtProtocolNotificationEvent) {
    gBS->CloseEvent(AcpiSdtProtocolNotificationEvent);
    AcpiSdtProtocolNotificationEvent = NULL;
  }
   for (Index = 0; Index < USB_CORE_MAX_NUM_SDM_1000; Index++) {
      UsbCoreIfc = gUsbConfigDevice[Index].UsbCoreInterface;

      // if HostModeSupported is FALSE, the UEFI image is used for the OS where HS
      // PHY can be configured entirely in the HLOS. We simply put the PHY in the
      // Non-Drive Mode.
      if (!FeaturePcdGet(HostModeSupported)) {
        UsbCoreIfc->SetNonDrivingMode(UsbCoreIfc, TRUE);
      }
      else {
        // if HostModeSupported is TRUE, the UEFI image is used for the OS where the
        // debugger software does not re-initialize the HS PHY in general.
        // If the current mode in UEFI is host mode, nothing needs to be done.
        // else we need to not only power down the PHY but also configure it.
        if (gUsbConfigDevice[Index].EnableStaticCfg.Enabled == TRUE &&
           gUsbConfigDevice[Index].EnableStaticCfg.UsbMode == USB_HOST_MODE_XHCI) {
          DBG(EFI_D_INFO, "Host mode: Don't power down the HS PHY");
        }
        else {
          DBG(EFI_D_INFO, "Peripheral mode: power down the HS PHY");
          //UsbCoreIfc->InitHsPhyPwrDwn(UsbCoreIfc);
        }
      }


 
      UsbConfigCoresDeinit(gUsbConfigDevice[Index].UsbCoreInterface);
      UsbConfigPortsDeinit(gUsbConfigDevice[Index].UsbPortHandle);
  }

  // detach SMMU
  if(gSmmuIsSetup) {
    if (FeaturePcdGet(USBSmmuStage1Supported)) {
    	if (gSmmuUsb30Attached) {
      		gSmmuTable->domain_detach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-0", 0x0, 0x0);
    	}
    	if (gSmmuUsb31Attached) {
      		gSmmuTable->domain_detach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-1", 0x0, 0x0);
    	}
    	if (gSmmuUsb32Attached) {
      		gSmmuTable->domain_detach_func(gUsbSmmuDomain, (void *)"\\_SB_.USB3-2", 0x0, 0x0);
    	}
	}
    gSmmuTable->domain_delete_func(gUsbSmmuDomain);
    gUsbSmmuDomain = NULL;
  }

  FNC_LEAVE_MSG ();
  return Status;
}


/**

@brief Stop the USB controller.

@param [in] CoreNum            The assigned core number for the USB controller.
@param [in] ModeType           The current USB mode the USB controller operates on.

@retval EFI_SUCCESS            The operation succeeded.
@retval EFI_UNSUPPORTED        The operation failed.

**/
EFI_STATUS
EFIAPI UsbStopController(
  IN   QCOM_USB_CORE_NUM          CoreNum,
  IN   QCOM_USB_MODE_TYPE         ModeType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  if ((INVALID_CORE(CoreNum)) || (ModeType >= USB_INVALID_MODE))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("UsbStopController: Error - Core %d. ModeType %d. current mode:", CoreNum, ModeType);
  }

  if ((CoreNum == USB_CORE_2_SDM_1000) && (ModeType != USB_HOST_MODE_XHCI)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("CoreNum = %d,Error - Invalid ModeType %d", CoreNum, ModeType);
  }

  // If the client does not specify the HOST or Device mode type,
  // switch it to the appropriated one for the core
  if (ModeType == USB_HOST_MODE)
    ModeType = USB_HOST_MODE_XHCI;
  else if (ModeType == USB_DEVICE_MODE)
    ModeType = USB_DEVICE_MODE_SS;

  // If static configuration is enabled, use the static configuration value.
  if (gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled)
    ModeType = gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode;

  // USB3.0 primary supports XHCI and device mode only.
  if ((ModeType != USB_HOST_MODE_XHCI) && (ModeType != USB_DEVICE_MODE_SS)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("Error - Invalid ModeType %d", ModeType);
  }

  // check if the mode to be stopped is the one set before
  if (gUsbConfigDevice[CoreNum].UsbConfig.modeType != ModeType) {
    Status = EFI_NOT_STARTED;
    ERR_CHK("Error - Unable to stop controller on core %d (curr mode %d, target mode %d)",
      CoreNum,
      gUsbConfigDevice[CoreNum].UsbConfig.modeType,
      ModeType);
  }

  // Stop Peripheral/Host Stack
  DBG(EFI_D_INFO, "Stopping Core %d in mode: %d ", CoreNum, ModeType);
  if (ModeType == USB_DEVICE_MODE_SS) {
    ARG_CHK(gUsbConfigDevice[CoreNum].PeripheralHandle);

    Status = gBS->DisconnectController(gUsbConfigDevice[CoreNum].PeripheralHandle, NULL, NULL);
    ERR_CHK("Failed to disconnect QcomUsbConfig protocol peripheral handle with Error: %r", Status);
    gUsbConfigDevice[CoreNum].PeripheralHandle = NULL;
  }
  else {
    ARG_CHK(gUsbConfigDevice[CoreNum].HostHandle);

    Status = gBS->DisconnectController(gUsbConfigDevice[CoreNum].HostHandle, NULL, NULL);
    ERR_CHK("Failed to disconnect QcomUsbConfig protocol host handle with error: %r", Status);
    gUsbConfigDevice[CoreNum].HostHandle = NULL;
  }

  //clear out device struct
  gUsbConfigDevice[CoreNum].UsbConfig.modeType = USB_INVALID_MODE;

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**

@brief Start the USB controller.

@param [in] CoreNum            The assigned core number for the USB controller.
@param [in] ModeType           The current USB mode the USB controller operates on.

@retval EFI_SUCCESS            The operation succeeded.
@retval EFI_UNSUPPORTED        The operation failed.

**/
EFI_STATUS
EFIAPI UsbStartController(
  IN   QCOM_USB_CORE_NUM          CoreNum,
  IN   QCOM_USB_MODE_TYPE         ModeType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SltLbEnabled = 0;
  UINTN      UefiVarSize;

  FNC_ENTER_MSG ();

  // If primary core is set to operate in host mode, hijack request to enable device mode on primary and initialize
  // secondary core instead when SLT loopback is enabled.
  if ((CoreNum == USB_CORE_0) && (ModeType == USB_DEVICE_MODE_SS)) {
	
    UefiVarSize = sizeof(SltLbEnabled);
    Status = gRT->GetVariable(L"EnableSLTLoopback", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &SltLbEnabled);
    if (!EFI_ERROR(Status) && (SltLbEnabled != 0) && (gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode == USB_HOST_MODE_XHCI)) {
      CoreNum = USB_CORE_1;
      DBG(EFI_D_WARN, "SLT Loopback, initialize device mode on CoreNum %d", CoreNum);
    }    
  }

  DBG(EFI_D_WARN, "usb_start Core %d, Mode %d", CoreNum, ModeType);

  if ((INVALID_CORE(CoreNum)) || (ModeType >= USB_INVALID_MODE))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("UsbStartController: Error - Core %d. ModeType %d. current mode:", CoreNum, ModeType);
  }

  if ((CoreNum == USB_CORE_2_SDM_1000) && (ModeType != USB_HOST_MODE_XHCI)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("CoreNum = %d, Error - Invalid ModeType %d", CoreNum, ModeType);
  }

  // If the client does not specify the HOST or Device mode type,
  // switch it to the appropriated one for the core
  if (ModeType == USB_HOST_MODE)
    ModeType = USB_HOST_MODE_XHCI;
  else if (ModeType == USB_DEVICE_MODE)
    ModeType = USB_DEVICE_MODE_SS;

  //USB3.0 primary supports XHCI and device mode only (HS/SS)
  if ((CoreNum == USB_CORE_0_SDM_1000) && (ModeType != USB_HOST_MODE_XHCI) && (ModeType != USB_DEVICE_MODE_SS))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("UsbStartController: Error - attempt to set primary core to ModeType %d", ModeType); 
  }

  // If static configuration is enabled, use the static configuration value.
  if (gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled) {
    if (ModeType != gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode) {
      DBG(EFI_D_WARN, "Static Config Mode (%d) overrides Client Selected Mode (%d)",
          gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode,
          ModeType);
    }
    ModeType = gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode;
  }

  //if core configured in same mode, ignore duplicate request
  if (gUsbConfigDevice[CoreNum].UsbConfig.modeType == ModeType)
  {
   DBG(EFI_D_WARN, "CoreNum %d, ModeType %d - Duplicate Request", CoreNum, ModeType);
    Status = EFI_SUCCESS;
    goto ON_EXIT;
  }
  else
  {
    // if the current usb mode is valid, stop the current one
    if (gUsbConfigDevice[CoreNum].UsbConfig.modeType != USB_INVALID_MODE) {
      Status = UsbStopController(CoreNum, gUsbConfigDevice[CoreNum].UsbConfig.modeType);
      ERR_CHK("UsbStopController: Error - Unable to stop controller on core %d, Status =  (0x%x)", CoreNum, Status);
    }
  }

  DBG(EFI_D_INFO, "Starting Core %d in mode: %d", CoreNum, ModeType);

  gUsbConfigDevice[CoreNum].UsbConfig.coreNum = CoreNum;
  gUsbConfigDevice[CoreNum].UsbConfig.modeType = ModeType;

  // Install/Connect Peripheral/Host Handle
  if (ModeType == USB_DEVICE_MODE_SS)
  {
    if (gUsbConfigDevice[CoreNum].PeripheralHandle == NULL) {
      //if handle is NULL, new one is allocated
      Status = gBS->InstallMultipleProtocolInterfaces(&gUsbConfigDevice[CoreNum].PeripheralHandle,
        &gQcomUsbConfigProtocolGuid, &gUsbConfigDevice[CoreNum].UsbConfig, NULL);
      ERR_CHK("Failed to install USB_CONFIG protocol (Peripheral Client Handle) with error: %r", Status);
    }

    Status = gBS->ConnectController(gUsbConfigDevice[CoreNum].PeripheralHandle, NULL, NULL, TRUE);
    ERR_CHK("Failed to connect QcomUsbConfig protocol peripheral handle with error: %r", Status);
  }
  else {
    if (gUsbConfigDevice[CoreNum].HostHandle == NULL) {
      //if handle is NULL, new one is allocated
      Status = gBS->InstallMultipleProtocolInterfaces(&gUsbConfigDevice[CoreNum].HostHandle,
        &gQcomUsbConfigProtocolGuid, &gUsbConfigDevice[CoreNum].UsbConfig, NULL);
      ERR_CHK("Failed to install USB_CONFIG protocol (Host Client Handle) with error: %r", Status);
    }
    Status = gBS->ConnectController(gUsbConfigDevice[CoreNum].HostHandle, NULL, NULL, TRUE);
    ERR_CHK("Failed to connect QcomUsbConfig protocol host handle with error: %r", Status);
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  This routine toggles the USB mode configuration on the core specified by CoreNum

  @param [in] CoreNum         The assigned core number for the USB controller.

  @retval EFI_SUCCESS         The operation succeeded.
  @retval EFI_UNSUPPORTED     The operation failed.

**/
EFI_STATUS
EFIAPI UsbToggleControllerMode(
  IN   QCOM_USB_CORE_NUM          CoreNum
  )
{
  EFI_STATUS             Status       = EFI_SUCCESS;
  QCOM_USB_MODE_TYPE     CurrModeType = USB_INVALID_MODE;
  QCOM_USB_MODE_TYPE     NextModeType = USB_INVALID_MODE;

  FNC_ENTER_MSG ();

  if (INVALID_CORE(CoreNum)) {
   Status = EFI_INVALID_PARAMETER;
   ERR_CHK ( "UsbToggleControllerMode: Error - Invalid CoreNum %d", CoreNum);
  }

  if (gUsbConfigTemplate.modeType == USB_INVALID_MODE) {
    Status = EFI_NOT_READY;
    ERR_CHK ( "UsbToggleControllerMode: Error - ModeType is USB_INVALID_MODE");
  }

  // store the current mode type. UsbStopController will set the
  // gUsbConfigTemplate.modeType to USB_INVALID_MODE
  CurrModeType = gUsbConfigTemplate.modeType;

  // The new mode on the core:
  // For Sdm1000, only switch from USB_DEVICE_MODE_SS to USB_HOST_MODE_XHCI
  switch (CurrModeType) {
    case USB_DEVICE_MODE_SS:
    NextModeType = USB_HOST_MODE_XHCI;
    break;
    case USB_DEVICE_MODE_HS:
    case USB_HOST_MODE_XHCI:
    case USB_HOST_MODE_EHCI:
    NextModeType = USB_INVALID_MODE;
    break;
    default:
    NextModeType = USB_INVALID_MODE;
  }

  if (NextModeType == USB_INVALID_MODE) {
    Status = EFI_UNSUPPORTED;
    ERR_CHK("Error - Setting USB_INVALID_MODE (on core %d) is not supported", CoreNum);
  }

  DBG(EFI_D_VERBOSE, "Starting toggling controller mode on core %d from mode %d to %d", CoreNum, CurrModeType, NextModeType);

  Status = UsbStopController(CoreNum, CurrModeType);
  ERR_CHK ("UsbToggleControllerMode: Error - Unable to stop controller on core %d, Status =  (%r)", CoreNum, Status);

  Status = UsbStartController(CoreNum, NextModeType);
  ERR_CHK ("UsbToggleControllerMode: Error - Unable to start controller on core %d, Status =  (%r)", CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Returns maximum number of cores supported in the host mode

  @param [in]  Mode              USB mode
  @param [in]  MaxCoreNum        Number of cores supported

  @retval EFI_SUCCESS            The operation succeeded.
  @retval EFI_UNSUPPORTED        The operation failed.

**/
EFI_STATUS
EFIAPI
GetUsbMaxHostCoreNum(
  IN  QCOM_USB_MODE_TYPE  Mode,
  OUT UINT8               *MaxCoreNum
  )
{
  // for Sdm1000, the function is not used.
  // if a use case shows up, the function needs to be
  // revisited and provide the actual implementation.
  // Just return success with Max number of core supported by SDM1000
  *MaxCoreNum = USB_CORE_MAX_NUM_SDM_1000;
  return EFI_SUCCESS;
}


/**
@summary
Return the total number of USB cores in the system

@param [out] Count          Pointer to the count of the system USB cores.

@retval EFI_SUCCESS         The operation succeeded.
@retval Others              The operation failed.

**/
EFI_STATUS
UsbConfigLibGetCoreCount(
  IN   UINT8          *Count
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (!Count) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  *Count = USB_CORE_MAX_NUM_SDM_1000;

ON_EXIT:
  return Status;
}


/**
@summary
Return the total number of USB cores in the system

@param [in]  CoreNum        Core number
@param [out] ModeType       Pointer to the supported mode type

@retval EFI_SUCCESS         The operation succeeded.
@retval Other               The operation failed.

**/
EFI_STATUS
UsbConfigLibGetSupportedMode(
  IN  QCOM_USB_CORE_NUM          CoreNum,
  OUT QCOM_USB_MODE_TYPE         *ModeType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if ((INVALID_CORE(CoreNum)) || !(ModeType)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("GetUsbVbusState: Error - Invalid CoreNum passed: %d", CoreNum);
  }

  if (CoreNum == USB_CORE_2_SDM_1000)
  {
    *ModeType = USB_HOST_MODE;
  }
  else 
  {
    *ModeType = USB_DEVICE_MODE | USB_HOST_MODE;
  }

ON_EXIT:
  return Status;
}

/**
@summary
Update the lane number.

The function updates the lane number in the software. The new value is not updated
to the hardware in the function. 

@param [in]  CoreNum        Core number

@retval EFI_SUCCESS         The operation succeeded.
@retval Other               The operation failed.

**/
STATIC
EFI_STATUS
UpdateLaneNumber(
  IN QCOM_USB_CORE_NUM    CoreNum)
{
  EFI_STATUS              Status;
  UINT8                   LaneNum;
  USB_TYPEC_CC_OUT_STATUS CcStatus;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
  
  if (INVALID_CORE(CoreNum)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("GetUsbVbusState: Error - Invalid CoreNum passed: %d", CoreNum);
  }

  //As there is no Lane number for MultiPort and MUlti Port - Hardcoded, return Success
  if(CoreNum == USB_CORE_2_SDM_1000 )
  {
    Status = EFI_SUCCESS;
    DBG (EFI_D_WARN, "No Lane Number for Multi Port");
    goto ON_EXIT;
  }
  
  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  // Get the CC status from PMIC
  Status = UsbPortGetCcOutStatus(gUsbConfigDevice[CoreNum].UsbPortHandle, &CcStatus);
  WRN_CHK("UsbPortGetCcOutStatus failed with status %r. Assume CC1", Status);

  // decide the Lane number based on the CC status
  if (CcStatus == USB_TYPEC_CC_OUT_CC1)
    LaneNum = USB_SS_PHY_LANE_A;
  else if (CcStatus == USB_TYPEC_CC_OUT_CC2)
    LaneNum = USB_SS_PHY_LANE_B;
  else {
    LaneNum = USB_SS_PHY_LANE_A;
    WRN_CHK("CC out status is neither CC1 or CC2, but %d. Default to CC1", CcStatus);
  }
  
  DBG (EFI_D_INFO,"LaneNum = %d", LaneNum);

  // Update the lane number
  Status = UsbCoreIfc->SetPhyLane(UsbCoreIfc, LaneNum);
  ERR_CHK("Set CC line failed with error: %r on core %d", Status, CoreNum);

ON_EXIT:
  return Status;
}

/**
 Handle Attach or Detach event

@param [in]  CoreNum       The core for which the event is.
@param [in]  Message       Attach or Detach message
@param [in]  PortMode      The current mode of the port.

@retval EFI_SUCCESS        The operation succeeded
@retval Other              The operation failed.
**/
STATIC
EFI_STATUS
HandlePortPartnerXtach(
  IN QCOM_USB_CORE_NUM    CoreNum,
  IN USB_CFG_PORT_MESSAGE Message,
  IN USB_TYPEC_PORT_MODE  PortMode)
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
  QCOM_USB_MODE_TYPE UsbCfgMode = USB_DEVICE_MODE_SS;
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE ChargerType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID;
  BOOLEAN StCtrler = FALSE;

  FNC_ENTER_MSG();

  // Set the charger type to SDP for RUMI
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)) {
    DBG(EFI_D_INFO, "Set Charger Type to SDP for RUMI");
    ChargerType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP;
    PortMode = USB_TYPEC_PORT_MODE_UFP;
  } 

  if ((INVALID_CORE(CoreNum)) ||
      (Message != UsbCfgPortMsgPartnerAttach && Message != UsbCfgPortMsgPartnerDetach)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameter");
  }

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  if (Message == UsbCfgPortMsgPartnerAttach) { //Attach

    DBG(EFI_D_INFO, "Cable Attach");

    //Need to set static configuration setting based on PortMode detected during Attach
    if(gDualRoleEnable[CoreNum] == TRUE)
    {
      switch (PortMode)
      {
        case USB_TYPEC_PORT_MODE_DFP:
          gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled = TRUE;
          gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode = USB_HOST_MODE_XHCI;
          gUsbConfigDevice[CoreNum].UsbCoreInterface->ModeType = USB_HOST_MODE;
          UsbCfgMode = USB_HOST_MODE_XHCI;
          StCtrler = TRUE;
          gDualRoleEnable[CoreNum] = FALSE;
          break;

        case USB_TYPEC_PORT_MODE_UFP:
          gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled = TRUE;
          gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode = USB_DEVICE_MODE_SS;
          gUsbConfigDevice[CoreNum].UsbCoreInterface->ModeType = USB_DEVICE_MODE;
          UsbCfgMode = USB_DEVICE_MODE_SS;
          StCtrler = FALSE;
          gDualRoleEnable[CoreNum] = FALSE;
          break;

        default:
          DBG(EFI_D_INFO, "Unknown Port Mode Detected %d", PortMode);
          goto ON_EXIT;
      }
    }

    // Update the LaneNumber variable in software.
    Status = UpdateLaneNumber(CoreNum);
    WRN_CHK ("UpdateLaneNumber failed on Core %d: %r", Status, CoreNum);

    // Upon cable attach, we need to consider whether to reset the USB hardware
    // If IS_HARDWARE_CONFIUGRED is false, do nothing as the USB hardware will be
    // configured later. When the UsbConfigDxe is first loaded, 
    // IS_HARDWARE_CONFIUGRED(modeType) returns false as modeType remains in 
    // USB_INVALID_MODE until that StartController function is invoked.
    // For LA, StartController is invoked by the fastboot App.
    // For WP, StartController is invoked later in this function.
    if (IS_HARDWARE_CONFIUGRED(gUsbConfigDevice[CoreNum].UsbConfig.modeType)) {
      // If we are here, it indicates the USB stack is loaded. We do a full reinit if the
      // following conditions are met.
      // (1) The device is configured for the peripheral use case.
      //   Host mode requires XHCIDxe changes which may not worth the effort as
      //   the USB host mode use case is limited. 
      //   The reinit will also bring HS PHY out of non-driving mode.
      // (2) The local port is UFP.
      //   Even if the USB hardware is configured as the peripheral mode, it does not prevent the USER
      //   to plug in a host cable. In that case, PMI charger detection is also not needed.
      // (3) The charger type is SDP or CDP
      //   If we do not need to enumerate the USB device, HS PHY can stay in the power down state.
      if (IS_USB_PERIPHERAL_MODE(gUsbConfigDevice[CoreNum].UsbConfig.modeType))
      { 
        Status = GetChargerPortType(CoreNum, &ChargerType);
        if (EFI_ERROR(Status)) { // cannot get charger type, assume invalid
          WRN_CHK("Cannot Get Charger Type");
        }
        if (IS_PORT_UFP(PortMode) && IS_SDP_CDP(ChargerType)) {
          // For the Device Mode, reinit ONLY the USB Link and PHY. Keep the clocks and power rails on.
          Status = UsbCoreIfc->InitCommon(UsbCoreIfc,TRUE);
          ERR_CHK ("ConfigUsb: Error - failed to perform common initialization for USB core %d: %r", CoreNum, Status);
              
          Status = UsbCoreIfc->InitDevice(UsbCoreIfc);
          ERR_CHK ("ConfigUsb: Error - failed to perform device initializaition for USB core %d: %r", CoreNum, Status);
        }
      }
      // for host mode, we still need to reset the SS PHY in order to update the lane number
      else if (IS_USB_HOST_MODE(gUsbConfigDevice[CoreNum].UsbConfig.modeType))
      {
        if(!(CoreNum == USB_CORE_0_SDM_1000 && gHighSpeedOnlyPrimary)
          || !(CoreNum == USB_CORE_1_SDM_1000 && gHighSpeedOnlySecondary)
          || !(CoreNum == USB_CORE_2_SDM_1000 && FeaturePcdGet(HighSpeedOnlySupported)))
        {
          Status = UsbCoreIfc->ResetSSPhy(UsbCoreIfc);
          ERR_CHK("SS PHY Reset failed with status %r", Status);
        }
      }
      else {
        DBG(EFI_D_WARN, "UsbModeType: %d",gUsbConfigDevice[CoreNum].UsbConfig.modeType);
      }
    }

    // For the HOST mode, call StartController automatically here as there is 
    // no client to perform the task.
    if (FeaturePcdGet(InitUsbControllerOnBoot)) {
      // if static cfg for the host mode is set, always start the controller.
      if (gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled == TRUE &&
          gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode == USB_HOST_MODE_XHCI) {
          StCtrler = TRUE;
      }

      // Start controller in Peripheral mode as default.
      // If gUsbConfigDevice[CoreNum].EnableStaticCfg.Enabled is TRUE, the mode
      // defined by gUsbConfigDevice[CoreNum].EnableStaticCfg.UsbMode will override
      // the one we set for UsbCfgMode.
      if (StCtrler) {
        DBG(EFI_D_INFO, "Start Controller");
        Status = UsbStartController(CoreNum, UsbCfgMode);
        ERR_CHK("Start controller failed with error: %r on core %d", Status, CoreNum);
      }
    }
  }
  else { // Detach
    // do nothing as dynamic role switching based on CC status is not supported.
    DBG(EFI_D_INFO, "Cable Detach");
 
    if (IS_USB_HOST_MODE(gUsbConfigDevice[CoreNum].UsbConfig.modeType))   
    {  
      Status = gPwrCtrlProtocol->SetVbusSourceEn(CoreNum, FALSE);      
      WRN_CHK("Disabling VBUS on Core Number: %d, Status: %r", CoreNum, Status);   
    }   
    
    // if the device is in the peripheral mode, put the HS PHY into non-driving
    // after detach.
    if (IS_USB_PERIPHERAL_MODE(gUsbConfigDevice[CoreNum].UsbConfig.modeType)) {
      // after cable detach, put the HS into non-driving
      UsbCoreIfc->SetNonDrivingMode(UsbCoreIfc, TRUE);
    }
  }
ON_EXIT:
  if (Status != EFI_SUCCESS)
    DBG(EFI_D_ERROR, "Error encountered in UsbPortStateCbGetCcStatus");

  FNC_LEAVE_MSG();
  return Status;
}


// Check UsbConfigLibPrivate.h for the description
STATIC
VOID
EFIAPI
UsbPortStateChgCb(
  IN EFI_EVENT       Event,
  IN VOID           *Context
  )
{
  EFI_STATUS Status;
  UINTN Index;
  USB_CFG_PORT_MESSAGE Message;
  UINTN PayloadSize;
  USB_TYPEC_PORT_MESSAGE_PAYLOAD Payload;
  QCOM_USB_CORE_NUM CoreNum;
  QCOM_USB_CONNECTOR_TYPE ConnectorType;
  UINT8 PortIndex;

  FNC_ENTER_MSG();

  for (Index = 0; Index < port_hw_info.NumOfPorts; Index++) {

    PortIndex = port_hw_info.PortIndexes[Index];

    Status = UsbPortGetInfo(PortIndex, &CoreNum, &ConnectorType);
    WRN_CHK("UsbPortGetInfo failed %r", Status);

    // Skip non type-c ports
    if (ConnectorType != USB_CONNECTOR_TYPE_C)
    {
      continue;
    }

    Status = UsbConfigPortsEventHandler(
      gUsbConfigDevice[CoreNum].UsbPortHandle,
      &Message,
      &PayloadSize,
      &Payload,
      PortIndex);

    switch (Message) {
    case UsbCfgPortMsgPartnerAttach:
    case UsbCfgPortMsgPartnerDetach:
      Status = HandlePortPartnerXtach(CoreNum, Message, Payload.UsbTypeCPortMode);
      WRN_CHK("HandlePortPartnerXtach failed %r", Status);
    default:
      break;
    }
  }

  FNC_LEAVE_MSG();
}

/**
Return connection state for USB Type-C Port

@param [in]  CoreType      The assigned the core type for the controller.
@param [out] IsConnected   Is the port partner connected.

@retval EFI_SUCCESS   The operation succeeded
@retval Other         The operation failed.

**/
EFI_STATUS
EFIAPI
GetUsbFnConnStatus(
  IN  QCOM_USB_CONFIG_CORE_TYPE    CoreType,
  OUT BOOLEAN                     *IsConnected
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  USB_TYPEC_PORT_MODE PortMode;
  QCOM_USB_CORE_NUM CoreNum;

  FNC_ENTER_MSG();

  if (NULL == IsConnected || (CoreType != USB_CONFIG_SSUSB1 && CoreType != USB_CONFIG_SSUSB2 && CoreType != USB_CONFIG_SSUSB3)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: CoreType %d, IsUFPMOD %p", CoreType, IsConnected);
  }

  Status = GetCoreNum(CoreType, &CoreNum);
  ERR_CHK(" Cannot locate CoreNum for CoreType %d: %r", CoreType, Status);
  if (INVALID_CORE(CoreNum)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("GetUsbVbusState: Error - Invalid CoreNum passed: %d", CoreNum);
  }

  //Assuming Seondary Port always connected - Hardcoded
  if(CoreNum == USB_CORE_1)
  {
    Status       = EFI_SUCCESS;
    *IsConnected = TRUE;
    goto ON_EXIT;
  }
  
  //Multi port support only Host Mode.
  if(CoreNum == USB_CORE_2)
  {
    Status       = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }
  
  // Check the mode of the port
  Status = UsbPortGetConnectionMode(gUsbConfigDevice[CoreNum].UsbPortHandle, &PortMode);
  ERR_CHK(" Cannot Get Connection Mode for Core %d: %r", CoreNum, Status);

  *IsConnected = (PortMode == USB_TYPEC_PORT_MODE_UFP || PortMode == USB_TYPEC_PORT_MODE_DFP) ? TRUE : FALSE;

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}

/**
Returns the USB Vbus State for the Core

@param [in]  CoreNum      Assigned CoreNumber
@param [out] VbusStatus   Vbus State

@retval EFI_SUCCESS      The operation succeeded
@retval  EFI_UNSUPPORTED Unsupported API
@retval Other            The operation failed.

**/

EFI_STATUS
EFIAPI
GetUsbVbusState (
  IN  QCOM_USB_CORE_NUM          CoreNum,
  OUT UINT32                     *VbusStatus
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
  USB_TYPEC_PORT_MODE PortMode;

  FNC_ENTER_MSG ();
   
  // For Clamshell platform Vbus state cannot be queried by PMIC
  if (IS_CLS_PLAT(PlatformInfo.platform)|| IS_RUMI_PLAT(PlatformInfo.platform)) {
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("GetUsbVbusState: Error - Invalid CoreNum passed: %d", CoreNum);
  }
   
  DBG(EFI_D_INFO,"CoreNum %d", CoreNum);
   
  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;
   
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc), EFI_DEVICE_ERROR);

  if ((CoreNum == USB_CORE_0) || (CoreNum == USB_CORE_1)) {
      // Check the mode of the port
      Status = UsbPortGetConnectionMode(gUsbConfigDevice[CoreNum].UsbPortHandle, &PortMode);
      ERR_CHK("Cannot Get Connection Mode for Core %d: %r", CoreNum, Status);

      // GetVbus for the Core if HOST cable is connected
      if(PortMode == USB_TYPEC_PORT_MODE_DFP)
      {
        USB_ASSERT_RETURN ((NULL != UsbCoreIfc->GetVbusStatus), EFI_DEVICE_ERROR);
        Status = UsbCoreIfc->GetVbusStatus(UsbCoreIfc, VbusStatus);
        ERR_CHK ("ConfigUsb: Error - failed to get the Vbus state for the USB core %d: %r",
            CoreNum, Status);
      }
      else //Unknown Port
      { 
        Status = EFI_UNSUPPORTED;
        goto ON_EXIT;
      }
    } // USB Regular Port (non Type-C)
  else {
      Status = UsbCoreIfc->GetVbusStatus(UsbCoreIfc, VbusStatus);
      ERR_CHK ("ConfigUsb: Error - failed to get the Vbus state for the USB core %d: %r",
          CoreNum, Status);

  }
ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}

/**
Enables the USB Vbus for the core

@param [in]  CoreNum      Assigned CoreNumber


@retval EFI_SUCCESS     The operation succeeded
@retval EFI_UNSUPPORTED Unsupported API
@retval Other           The operation failed.

**/

EFI_STATUS
EFIAPI
UsbEnableVbus (
  IN  QCOM_USB_CORE_NUM          CoreNum
  )
{

  EFI_STATUS              Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;
 
  FNC_ENTER_MSG ();
 
   // For Clamshell platform Vbus state cannot be enabled by PMIC
  if (IS_CLS_PLAT(PlatformInfo.platform) || IS_RUMI_PLAT(PlatformInfo.platform)) {
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("ConfigUsb: Error - Invalid CoreNum passed: %d", CoreNum);
  }
 
  DBG(EFI_D_INFO,"CoreNum %d", CoreNum);
 
  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;
 
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc), EFI_DEVICE_ERROR);
 
 // Enable the Vbus for the Core
 USB_ASSERT_RETURN ((NULL != UsbCoreIfc->EnableVbus), EFI_DEVICE_ERROR);
 Status = UsbCoreIfc->EnableVbus(UsbCoreIfc);
 ERR_CHK ("ConfigUsb: Error - failed to enable the Vbus for the USB core %d: %r",
     CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
Polls SS USB PHY for handling special training sequence

@param [in]  CoreNum      Assigned CoreNumber


@retval EFI_SUCCESS     The operation succeeded
@retval EFI_UNSUPPORTED Unsupported API
@retval Other           The operation failed.

**/

EFI_STATUS
EFIAPI
PollSSPhyTraining (
  IN  QCOM_USB_CONFIG_CORE_TYPE    CoreType
  )
{

  EFI_STATUS              Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;

  QCOM_USB_CORE_NUM CoreNum;

  FNC_ENTER_MSG();

  if (chipVersion >= EFI_CHIPINFO_VERSION(2,0))
  {
    // Not needed for chip revision 2.0 or higher.
    goto ON_EXIT;
  }

  if (CoreType != USB_CONFIG_SSUSB1 && CoreType != USB_CONFIG_SSUSB2) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: CoreType %d", CoreType);
  }

  Status = GetCoreNum(CoreType, &CoreNum);
  ERR_CHK("Cannot locate CoreNum for CoreType %d: %r", CoreType, Status);
  if (INVALID_CORE(CoreNum)) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Error - Invalid CoreNum passed: %d", CoreNum);
  }

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;

  USB_ASSERT_RETURN ((NULL != UsbCoreIfc), EFI_DEVICE_ERROR);

  // Enable the Vbus for the Core
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc->PollSSPhyTraining), EFI_DEVICE_ERROR);
  Status = UsbCoreIfc->PollSSPhyTraining(UsbCoreIfc);
  ERR_CHK ("Error - failed to poll for USB core %d: %r",
   CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
Advances USB SS compliance pattern through hardware register.
@param [in]  CoreNum      Assigned CoreNumber


@retval EFI_SUCCESS     The operation succeeded
@retval EFI_UNSUPPORTED Unsupported API
@retval Other           The operation failed.

**/

EFI_STATUS
EFIAPI
AdvanceSSCmplPattern (
  IN  QCOM_USB_CORE_NUM    CoreNum
  )
{

  EFI_STATUS              Status = EFI_SUCCESS;
  QCOM_USB_CORE_INTERFACE *UsbCoreIfc;

  FNC_ENTER_MSG();

  if (INVALID_CORE(CoreNum))
  {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK ("ConfigUsb: Error - Invalid CoreNum passed: %d", CoreNum);
  }

  UsbCoreIfc = gUsbConfigDevice[CoreNum].UsbCoreInterface;
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc), EFI_DEVICE_ERROR);

  // Advance pattern on the DWC3 hardware using GUSB3PIPECTL.HstPrtCmpl bit 
  USB_ASSERT_RETURN ((NULL != UsbCoreIfc->AdvanceSSCmplPattern), EFI_DEVICE_ERROR);
  Status = UsbCoreIfc->AdvanceSSCmplPattern(UsbCoreIfc);
  ERR_CHK ("Error - failed to advance pattern on USB core %d: %r", CoreNum, Status);

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
Function to patch DSDT ACPI table 

@param [in] Table                ACPI table pointer
@param [in] Version              Version
@param [in] TableKey             TableKey

@return
  EFI_SUCCESS        Function completed successfully. 
  Other              Error occurred during the operation.

**/
EFI_STATUS
UpdateDsdtTable(
  IN EFI_ACPI_SDT_HEADER *Table,  
  IN EFI_ACPI_TABLE_VERSION Version,  
  IN UINTN TableKey )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  EFI_ACPI_HANDLE       AcpiHandle = NULL;
  EFI_ACPI_HANDLE       OutHandle = NULL;
  EFI_ACPI_DATA_TYPE    DataType;
  CONST UINT8           *Data;
  UINT8                 isEnable = 0x1;          //set to enable
  UINTN                 i = 0, max_iter = 10;
  UINTN                 DataSize;

  //open handle with top node of DSDT table
  Status = gAcpiSdt->Open((VOID*)(UINTN)Table+sizeof(EFI_ACPI_SDT_HEADER),&AcpiHandle);
  if (EFI_ERROR (Status) || !AcpiHandle)
  {
    DEBUG((EFI_D_ERROR, "[UsbConfig]UpdateDsdtTable: Error opening the handle for DSDT tables \r\n"));
    goto ON_EXIT;
  }

  Status = gAcpiSdt->FindPath(AcpiHandle,(UINT8*)EFI_DEVICE_USB_QC_COMPOSITE,&OutHandle);
  if(EFI_ERROR(Status) || !OutHandle)
  {
    DEBUG((EFI_D_ERROR, "[UsbConfig] Error gAcpiSdt->FindPath, Status = (0x%x) or NULL handle \r\n", Status));
  }
  else
  {
    // check if this is a NAME opcode
    Status = gAcpiSdt->GetOption(OutHandle, i, &DataType, (CONST VOID **) &Data, &DataSize);
    if (EFI_ERROR(Status) || !Data || (DataType != EFI_ACPI_DATA_TYPE_OPCODE) || (Data[0] != AML_NAME_OP)) 
    {
      DEBUG((EFI_D_ERROR, "[UsbConfig] Error finding data (opcode for name operation)\n"));
      goto ON_EXIT;
    }

    // go through indices until we find the value of the global var
    while ( (DataType != EFI_ACPI_DATA_TYPE_CHILD) && (i < max_iter) )
    {
      i++;
      Status = gAcpiSdt->GetOption(OutHandle, i, &DataType, (CONST VOID **) &Data, &DataSize);
      if (EFI_ERROR(Status)) 
      {
        DEBUG((EFI_D_ERROR, "[UsbConfig] Error finding data\n"));
      }
    }

    if (Data[0] != isEnable) 
    {
      Status = gAcpiSdt->SetOption(OutHandle, i, &isEnable, sizeof(isEnable));
      if ((EFI_ERROR(Status))) 
      {
        DEBUG((EFI_D_ERROR, "[UsbConfig] Error setting ACPI value\n"));
        goto ON_EXIT;
      }
    }
  }

ON_EXIT:
  if (AcpiHandle)
  {
    gAcpiSdt->Close(AcpiHandle);
  }
  if (OutHandle)
  {
    gAcpiSdt->Close(OutHandle);
  }
  
  return Status;
}


/**
Callback Function used for ACPI DSDT table patching

@param [in] Table                ACPI table pointer
@param [in] Version              Version
@param [in] TableKey             TableKey

@return
  EFI_SUCCESS        Function completed successfully. 
  Other              Error occurred during the operation.

**/
static 
EFI_STATUS
EFIAPI
AcpiNotification (  
	IN EFI_ACPI_SDT_HEADER *Table,  
	IN EFI_ACPI_TABLE_VERSION Version,  
	IN UINTN TableKey)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (Table == NULL) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("[UsbConfig] ACPI table pointer == NULL \r\n");
  }
  
  //check if table is indeed DSDT
  if (Table->Signature == EFI_ACPI_6_1_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE){
    Status = UpdateDsdtTable(Table,Version,TableKey);
  }

ON_EXIT:
  return Status;
}


/**
Callback of SDT protocol installed used to register for ACPI table load notification

@param [in] Event                The event that is triggered.
@param [in] Context              The context to the event.

@return                          none

**/
VOID
AcpiSdtProtocolInstalledNotification (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  // Locate ACPI SDT Protocol and register for callback for loading of ACPI tables 
  Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, (VOID**) &gAcpiSdt);

  if (EFI_ERROR (Status) || (!gAcpiSdt)) {
    DEBUG((EFI_D_ERROR, "[UsbConfig]Failed to Locate SDT protocol \r\n"));
  }
  else
  {
    // Register callback for when any ACPI table is loaded- used only for DSDT table patching
    gAcpiSdt->RegisterNotify(TRUE,AcpiNotification);
  }
}


/**
Function to register for SDT protocol installation

@param none

@return
  EFI_SUCCESS        Function completed successfully. 
  Other              Error occurred during the operation.

**/
EFI_STATUS
EFIAPI
UsbConfigUpdateACPIEntry ()
{
  EFI_STATUS Status = EFI_SUCCESS;   

  // Set up to be notified when the ACPI SDT protocol is installed.
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, AcpiSdtProtocolInstalledNotification, NULL, &AcpiSdtProtocolNotificationEvent);    
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (&gEfiAcpiSdtProtocolGuid, AcpiSdtProtocolNotificationEvent, (VOID *)&AcpiSdtProtocolNotificationToken);
  ASSERT_EFI_ERROR (Status); 

  return Status;
}
