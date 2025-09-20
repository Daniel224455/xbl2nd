#ifndef __APPCMDS_H__
#define __APPCMDS_H__

/*=============================================================================

                     App commands interface Header File

  GENERAL DESCRIPTION
    Implementation of various App Commands for CmdApp

*
 *
*//*========================================================================*/
//============================================================================
/**
  Copyright (c) 2016-2019 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================
// ===========================================================================*/
//


/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


  when       who     what, where, why
  --------   ---     ---------------------------------------------------------
  03/15/19   mr      Add option to 'verify FW Update feature'
  12/13/18   ivy     support pmic ship mode
  05/22/18   mt      Add TogglePxeBoot
  10/13/18   my      Add option to trigger a NS WDOG bite
  10/11/18   kameya  Support USB SLT loopback feature.
  09/27/18   ivy     support fg ship mode
  08/17/18   gk      Make Superspeed MassStorage as Default setting
  08/07/18   yw      add option to toggle hw wdog, add MountFv GUID support
  06/11/18   sb      Add Toggle DISABLEBATTERY UEFI Variable through BDS MENU
  02/13/18   gk      Port changes from boot.xf.2.0 and boot.xf.2.2
  01/22/18   gk      Added USB Variable to Enable High Speed Mode For Display port
  01/08/18   op      Fix Clocks'state dumpping
  12/12/17   pm      Added support for device mode on secondary port
  09/27/17   vsb     Add QcUsbFN HLOS override enable
  09/08/17   kpa     Added support for MountFv tool
  09/04/17   gk      UEFI USB compliance test changes
  07/28/17   pm      Added support to set UsbHLOSPortRole value
  03/14/17   op      Add Clocks Menu
  06/22/16   ck      Remove the option to use secondary port for the debugger
  03/22/16   bh      Adding BootOrder creation for 64-bit BDS testing
  11/18/15   va      UEFI Factoy ship mode support
  08/13/15   al      Adding Charging related
  05/28/15   rli     Add toggle for UEFI File Logging
  10/02/13   ck      Add SetBootUsbFirstDelayValue
  08/29/13   ml      Added support for SSUsbfn
  05/13/13   niting  Add options to support memory dump
  05/12/13   niting  Change HotKey Menu option
  05/02/13   sb      Add ToggleUseUsbFnPrimPort
  04/03/13   sm      Editted description for DisableChgApp
  04/03/13   yg      Added Low power command
  03/20/13   nk      Added XPU Error Fatal
  02/20/13   niting  Added flag to toggle volatile boot options
  02/15/13   niting  Added ability to keep display ON during charging
  02/15/13   dy      Add ToggleDisplay
  02/14/13   mliss   Add ToggleUseSynopsys
  02/07/13   dy      Add Toggle Battery Rsense
  11/13/12   dy      Add Disable Chg App
  09/17/12   yg      Add Disable Hotkey
  08/22/12   yg      Add Boot USBFirst
  08/22/12   vk      Add Toggle Enable Shell
  07/27/12   yg      Convert Disable to toggle function
  07/24/12   yg      Add disable write protection function
  07/17/12   vo      Initial creation

============================================================================*/

/*
 * Handler for DispInfo Command
 */
EFI_STATUS
DispInfo(CHAR8* Arg);

/*
 * Handler for ToggleWriteProtect Command
 */
EFI_STATUS
ToggleWriteProtection (CHAR8* Arg);

/*
 * Handler for ToggleEnableShell Command
 */
EFI_STATUS
ToggleEnableShell(CHAR8* Arg);

/*
 * Handler for Boot USBFirst Command
 */
EFI_STATUS
BootUSBFirst(CHAR8* Arg);

/*
 * Handler to toggle the BDSHotKeyState variable.
 */
EFI_STATUS
ToggleBDSHotKeyState (CHAR8* Arg);

/*
 *  Toggle Clock  status File Logging setting
 */
EFI_STATUS
ToggeClockLoggingState (CHAR8* Arg);

/*
 *  Reset Clock status File Logging setting to 0
 */
EFI_STATUS
ResetClockLoggingState (CHAR8* Arg);

/*
 *  Dump Clock status to  UART
 */

EFI_STATUS
DumpClockLoggingState (CHAR8* Arg);

/*
 *  Dump CPU Clock cluster plan to UART
 */
EFI_STATUS
DumpCPUClocksPlan (CHAR8* Arg);

/*
 *  Get CPU Clock cluster current frequency
 */
EFI_STATUS
GetCPUClocksFrequency (CHAR8* Arg);

/*
 *  Set CPU Clock cluster frequency
 */
EFI_STATUS
SetCPUClocksFrequency (CHAR8* Arg);


/*
 * Handler to Toggle charger power-on trigger
 */
EFI_STATUS
DisableChgApp (CHAR8* Arg);

/*
 * Handler to Toggle Battery Rsense
 */
EFI_STATUS
ConfigRsense (CHAR8* Arg);

/*
 * Handler for SetUsbConfigPrimaryPort Command
 */
EFI_STATUS
SetUsbConfigPrimaryPort (CHAR8* Arg);

/*
 * Handler for SetUsbConfigSecondaryPort Command
 */
EFI_STATUS
SetUsbConfigSecondaryPort (CHAR8* Arg);


/*
 * Handler for ToggleEnableHsUartBaudRate Command
 */
EFI_STATUS
ToggleEnableHsUartBaudRate (CHAR8* Arg);

/*
 * Handler for ToggleHSUsbfnForceHS Command
 */
EFI_STATUS
ToggleHSUsbfnForceHS (CHAR8* Arg);

/*
 * Handler for SetUsbfnMaxSpeed Command
 */
EFI_STATUS
SetUsbfnMaxSpeed(CHAR8* Arg);
/*
 * Handler for ToggleForceUsbDeviceModeCompliance Command
 */
EFI_STATUS
ToggleForceUsbDeviceModeCompliance (CHAR8* Arg);

/*
 * Handler for SetBootUsbFirstDelayValue Command
 */
EFI_STATUS
SetBootUsbFirstDelayValue(CHAR8 *Arg);

/*
 * Handler for ToggleForceQcUsbFnHlos Command for Primary Port only.
 */
EFI_STATUS
ToggleForceQcUsbFnHlos (CHAR8* Arg);

/*
 * Handler for ToggleEnableSLTLoopback Command.
 */
EFI_STATUS
ToggleEnableSLTLoopback (CHAR8* Arg);

/*
 * Handler to Toggle Display During Charging
 */
EFI_STATUS
ToggleDisplayDuringCharging (CHAR8* Arg);

/*
 * Handler to Toggle Enable Volatile Boot Options
 */
EFI_STATUS
ToggleEnableVolatileBootOptions (CHAR8* Arg);

/*
 * Handler to Toggle EnableOnstorageDump
 */
EFI_STATUS
ToggleEnableOnstorageDump (CHAR8* Arg);

/*
 * Handler to Toggle OfflineMemoryDumpUseCapability
 */
EFI_STATUS
ToggleOfflineMemoryDumpUseCapability (CHAR8* Arg);

/*
 * Handler to EnableHSUsbPrimary for Primary Port only.
 */
EFI_STATUS
EnableHSUsbPrimary (CHAR8* Arg);

/*
 * Handler to EnableHSUsbSecondary for Secondary Port only.
 */
EFI_STATUS
EnableHSUsbSecondary (CHAR8* Arg);

/*
 * Handler to EnableUsbDPBondedPrimary for Primary Port only.
 */
EFI_STATUS
EnableUsbDPBondedPrimary (CHAR8* Arg);

/*
 * Handler to EnableUsbDPBondedSecondary for Secondary Port only.
 */
EFI_STATUS
EnableUsbDPBondedSecondary (CHAR8* Arg);

/*
 *  Enable/Disable XPU Error Fatals
 */
EFI_STATUS
XpuErrFatal (CHAR8* Arg);

/*
 *  Low Power Mode
 */
EFI_STATUS
LowPowerMode (CHAR8* Arg);

/*
 * Handler to Toggle printing debug msgs from Charger App
 */
EFI_STATUS
PrintChargerAppDbgMsg (CHAR8* Arg);


/*
 *  Toggle UEFI File Logging
 */
EFI_STATUS
FileLoggingToggle (CHAR8* Arg);

/*
 *  Toggle Qcom charger app
 */
EFI_STATUS
RunQcomChgrApp (CHAR8* Arg);

/*
 * Handler to Toggle Ship Mode
 */
EFI_STATUS
EnPmicShipMode ();

VOID
Add64BitBootOrder(CHAR8* Arg);

/*
 * Handler to command to mount a FV
 */
EFI_STATUS
MountFvCommand (UINTN  Argc, CHAR8  **Argv);

/*
 * Force enable HW wdog
 */
EFI_STATUS
ToggleHWWdog(VOID);

/*
 * Toggle DISABLEBATTERY UEFI variable
 */
EFI_STATUS
ToggleDISABLEBATTERY (CHAR8* Arg);

/*
 * Handler to Toggle MemMap Display
 */
EFI_STATUS
ToggleEnableMemMap (CHAR8* Arg);

/*
 * Toggle FG ship mode
 */
EFI_STATUS
EnFgShipMode ();

/*
 * Toggle FW Update support UEFI variable for verification
 */
EFI_STATUS
TestFwUpdateSupport ();

/*
 * Trigger a non-secure WDOG bite
 */
EFI_STATUS
TriggerWdogBite(VOID);

/*
 * Handler to Toggle PXE Boot
 */
EFI_STATUS
TogglePxeBoot (CHAR8* Arg);

/*
 * Toggle Off Mode Sourcing
 */
EFI_STATUS
EnOffModeSrc();


/*
 * Toggle HW Auto mode for Off Mode Sourcing
 */
EFI_STATUS
EnHWAutoOffModeSrc();

#endif  /* __APPCMDS_H__ */

