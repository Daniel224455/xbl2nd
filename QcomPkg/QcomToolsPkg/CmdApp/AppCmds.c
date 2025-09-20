/** @file AppCmds.c

  Module to implement app commands as helper functions to CmdApp
*
 *
*//*========================================================================*/
//============================================================================
/**
  Copyright (c) 2016-2019, 2022 Qualcomm Technologies, Inc. All rights reserved.
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
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/15/19   mr      Add option to 'Verify FW Update feature'
 12/13/18   ivy     support pmic ship mode and update for fg ship mode
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
 06/22/16   ck      Remove the option to use secondary port for the debugger
 03/22/16   bh      Adding BootOrder creation for 64-bit BDS testing
 11/18/15   va      UEFI Factoy ship mode support
 10/08/15   bh      Change default delay to 30s for desktop
 08/13/15   al      Adding Charging related
 06/25/15   bh      Change guid name
 05/28/15   rli     Add toggle for UEFI File Logging
 10/02/13   ck      Add SetBootUsbFirstDelayValue
 08/29/13   ml      Added support for SSUsbfn
 06/20/13   yg      Small improvement to range config
 05/13/13   niting  Add options to support memory dump
 05/12/13   niting  Change HotKey Menu option
 05/09/13   yg      Add low power tests
 05/02/13   sb      Add ToggleUseUsbFnPrimPort
 04/03/13   sm      Editted DisableChgApp API definition
 04/03/13   yg      Added LowPowerMode command
 03/20/13   nk      Added XPU Error Fatal
 02/20/13   niting  Added flag to toggle volatile boot options
 02/15/13   niting  Added ability to keep display ON during charging
 02/15/13   dy      Add ToggleDisplay
 02/14/13   mliss   Add ToggleUseSynopsys
 02/07/13   dy      Add ConfigRsense option
 12/10/12   yg      Handle -ve use cases better
 11/13/12   dy      Add Disable QC Charger App option
 09/17/12   yg      Add Disable Hotkey and some cleanup
 08/22/12   yg      Add Boot USBFirst
 08/22/12   vk      Add ToggleEnableShell
 07/27/12   yg      Convert Disable to toggle function
 07/24/12   yg      Add DisableWriteProtect function
 07/18/12   yg      Disable reset, cleanup of messages
 07/06/12   vishalo Initial Revision
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/QcomLib.h>
#include <Library/OfflineCrashDump.h>
#include "AppCmds.h"
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIChipInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/EFIQcomWDog.h>
#include <Library/DevicePathLib.h>
#include <Library/MountFv.h>

extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gStickyGuid;

#define INVERTED_MEANING         TRUE
#define NON_INVERTED_MEANING     FALSE


/* BDS nCPU ID type */
typedef enum {
  ///
  /// CPU Clock ID
  ///
  BDS_CPU_ID0        = 0,
  BDS_CPU_ID1        = 1,
  BDS_CPU_ID2        = 2,
  BDS_CPU_ID3        = 3,
  BDS_HANA_CPU_MAX  = BDS_CPU_ID3,
  BDS_CPU_ID4        = 4,
  BDS_CPU_ID5        = 5,
  BDS_CPU_ID6        = 6,
  BDS_CPU_ID7        = 7,
  BDS_CPU_ID8        = 8,
  BDS_POIPU_CPU_MAX   = BDS_CPU_ID8,
  ///
  /// Maximum enumeration value that may be used for bounds checking.
  ///
  MaxBDSCpuId
} BDS_CPU_ID_TYPE;

EFI_STATUS
QueryToggleFlag (CONST CHAR8* FeatureString, UINT8* Flagp, BOOLEAN Inverted)
{
  EFI_STATUS    Status;
  EFI_INPUT_KEY    Key;
  UINT8         FeatureFlag;

  if (Flagp == NULL)
    return EFI_INVALID_PARAMETER;

  FeatureFlag = *Flagp;
  if (Inverted)
  {
    if (*Flagp == 0)
      FeatureFlag = 1;
    else
      FeatureFlag = 0;
  }

  FlushKeys ();

  if (FeatureFlag == 0)
    AsciiPrint("\r\nFeature \"%a\" : Disabled\r\n", FeatureString);
  else
    AsciiPrint("\r\nFeature \"%a\" : Enabled\r\n", FeatureString);

  AsciiPrint("\r\nPress [UP] key to Enable, [DOWN] key to Disable, [Other] key NO change\r\n");

  Status = ReadAnyKey (&Key, 0);
  if (EFI_ERROR(Status))
    return EFI_DEVICE_ERROR;

  if (Key.ScanCode == 0)
    Key.ScanCode = Key.UnicodeChar;

  if (Key.ScanCode == SCAN_UP)
  {
    AsciiPrint("\r\nEnabling \"%a\"\r\n\r\n", FeatureString);
    *Flagp = 1;
  }
  else if (Key.ScanCode == SCAN_DOWN)
  {
    AsciiPrint("\r\nDisabling \"%a\"\r\n\r\n", FeatureString);
    *Flagp = 0;
  }
  else
  {
    AsciiPrint("\r\n\nNot confirmed, exiting.\r\n");
    return EFI_NO_MAPPING;
  }

  if (Inverted)
  {
    if (*Flagp == 0)
      *Flagp = 1;
    else
      *Flagp = 0;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
QuerySetFlag (CONST CHAR8* FeatureString, CONST CHAR8* ValueMapStr, INT8* Flagp, UINT8 MinVal, UINT8 MaxVal)
{
  EFI_STATUS    Status;
  EFI_INPUT_KEY    Key;
  INT8   PrevVal;

  if (Flagp == NULL)
    return EFI_INVALID_PARAMETER;

  FlushKeys ();

  PrevVal = *Flagp;

  AsciiPrint("\r\nFeature \"%a\" = %d\r\n\n", FeatureString, *Flagp);

  AsciiPrint("Value Map :\r\n%a\r\n", ValueMapStr);

  AsciiPrint("\r\nPress [UP] key to increment, [DOWN] key to decrement, [Other] key Done\r\n\n");

  AsciiPrint("\rNew Value: %d ", *Flagp);

  while (1)
  {
    Status = ReadAnyKey (&Key, 0);
    if (EFI_ERROR(Status))
      return EFI_DEVICE_ERROR;

    if (Key.ScanCode == 0)
      Key.ScanCode = Key.UnicodeChar;

    if (Key.ScanCode == SCAN_UP)
    {
      if (*Flagp < MaxVal)
      {
        *Flagp += 1;
        AsciiPrint("\rNew Value: %d ", *Flagp);

      }
    }
    else if (Key.ScanCode == SCAN_DOWN)
    {
      if (*Flagp > MinVal)
      {
        *Flagp -= 1;
        AsciiPrint("\rNew Value: %d ", *Flagp);
      }
    }
    else
    {
      AsciiPrint("\r\n");
      if (*Flagp == PrevVal)
      {
        AsciiPrint("No change, exiting\r\n");
        return EFI_NO_MAPPING;
      }
      break;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
QuerySetItem (CONST CHAR8* FeatureString, CONST CHAR8* ValueMapStr,INT8* Flagp, UINT8 MinVal, UINT8 MaxVal)
{
  EFI_STATUS    Status;
  EFI_INPUT_KEY    Key;
  INT8   PrevVal;

  if (Flagp == NULL)
    return EFI_INVALID_PARAMETER;

  if (MinVal == MaxVal)
  {
    AsciiPrint ("QuerySetItem: Min value equal to Max value %d \r\n", MinVal);
    return EFI_INVALID_PARAMETER;
  }

  FlushKeys ();

  PrevVal = *Flagp;
  if (ValueMapStr == NULL)
  {
    AsciiPrint("\r\nFeature \"%a\" [%d - %d]= %d\r\n\n", FeatureString,MinVal,MaxVal, *Flagp);
  }
  else
  {
    AsciiPrint("\r\nFeature \"%a\" = %d\r\n\n", FeatureString, *Flagp);
  }
  if (ValueMapStr!=NULL)
  {
    AsciiPrint("Value Map :\r\n%a\r\n", ValueMapStr);
  }

  AsciiPrint("\r\nPress [UP] key to increment, [DOWN] key to decrement, [Other] key Done\r\n\n");

  AsciiPrint("\rNew Value: %d ", *Flagp);

  while (1)
  {
    Status = ReadAnyKey (&Key, 0);
    if (EFI_ERROR(Status))
      return EFI_DEVICE_ERROR;

    if (Key.ScanCode == 0)
      Key.ScanCode = Key.UnicodeChar;

    if (Key.ScanCode == SCAN_UP)
    {
      if (*Flagp < MaxVal)
      {
        *Flagp += 1;
        AsciiPrint("\rNew Value: %d ", *Flagp);

      }
    }
    else if (Key.ScanCode == SCAN_DOWN)
    {
      if (*Flagp > MinVal)
      {
        *Flagp -= 1;
        AsciiPrint("\rNew Value: %d ", *Flagp);
      }
    }
    else
    {
      AsciiPrint("\rNew Value: %d ", *Flagp);
      break;
    }
  }
  AsciiPrint("\r\n");
  return EFI_SUCCESS;
}

EFI_STATUS
TogglePxeBoot (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnablePxeBoot", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("PXE Boot", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnablePxeBoot", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

EFI_STATUS
DispInfo(CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize = 0;

  VarSize = sizeof(Flag);

  Status = gRT->GetVariable (L"DispDebugInfo", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if (Status == EFI_NOT_FOUND)
    Flag = 0;

  if (QueryToggleFlag ("Display Info on Screen", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);

  Status = gRT->SetVariable (L"DispDebugInfo", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to store setting\r\n");

  return Status;
}

EFI_STATUS
ToggleWriteProtection (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"DisableWriteProtect", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("eMMC Write Protection", &Flag, INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"DisableWriteProtect", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

EFI_STATUS
ToggleEnableShell (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableShell", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Run Shell on Boot fail", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableShell", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

EFI_STATUS
ToggleEnableMemMap (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableMemMap", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Memory Map Display", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableMemMap", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}


EFI_GUID gOwnerGuid = {0x77fa9abd, 0x0359, 0x4d32, {0xbd, 0x60, 0x28, 0xf4, 0xe7, 0x8f, 0x78, 0x4b}};

EFI_STATUS
BootUSBFirst(CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"AttemptUSBFirst", &gOwnerGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Attempt USB First", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"AttemptUSBFirst", &gOwnerGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}


/*
 * Handler to toggle the BDSHotKeyState variable.
 */

#define  BDS_HOTKEY_STATE_MAP_STRING     " 0 = Detect HotKey"      "\r\n" \
                                         " 1 = Disable HotKey"     "\r\n" \
                                         " 2 = Force HotKey Menu"

EFI_STATUS
ToggleBDSHotKeyState (CHAR8* Arg)
{
  EFI_STATUS    Status;
  INT8          Flag = 0;
  UINTN         VarSize;
  UINT8         MaxVal = MaxBDSHotKeyState - 1;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (BDS_HOTKEY_STATE_VARNAME, &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QuerySetFlag ("BDS HotKey State", BDS_HOTKEY_STATE_MAP_STRING, &Flag, 0, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (BDS_HOTKEY_STATE_VARNAME, &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if (Status == EFI_SUCCESS)
    AsciiPrint ("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint ("ERROR: Unable to change/store setting\r\n");

  return Status;
}

#define  BDS_CLOCKLOG_STATE_MAP_STRING   " 0 = Disable Clock logging"   "\r\n" \
                                         " 1 = Uart Clock logging"

EFI_STATUS
ToggeClockLoggingState (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS    Status;
  INT8          Flag = 0;
  UINTN         VarSize;
  UINT8         MaxVal = MaxBDSClockLogState - 1;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (BDS_CLOCKLOG_STATE_VARNAME, &gQcomTokenSpaceGuid,
             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
     Flag = 0;

  if (QuerySetFlag ("BDS Clock Log State", BDS_CLOCKLOG_STATE_MAP_STRING, &Flag, 0, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (BDS_CLOCKLOG_STATE_VARNAME, &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);
  FlushVariableServices();

  if (Status == EFI_SUCCESS)
    AsciiPrint ("Clock log State setting success.\r\n");
  else
    AsciiPrint ("ERROR: Unable to change/store setting\r\n");

  return Status;
}

EFI_STATUS
ResetClockLoggingState (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS    Status;
  INT8          Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (BDS_CLOCKLOG_STATE_VARNAME, &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if (Status == EFI_SUCCESS)
  {
     AsciiPrint ("\r\nReSetLoggingState: %d\r\n", Flag);
    /* On successful write to variable services, trigger a flush to ensure it is written to disk without
       requring a reset. */
    FlushVariableServices();
  }
  else
    AsciiPrint ("\r\nERROR: Unable to change/store setting\r\n");

  return Status;
}


/*
 *  Dump Clock status to  UART
 */
EFI_STATUS
DumpClockLoggingState (CHAR8* Arg)
{
  EFI_STATUS    eStatus;
  INT8          Flag = 0;
  UINTN         VarSize;
  EFI_TEST_CLOCK_PROTOCOL *pTestClockProtocol;

  VarSize = sizeof(Flag);
  eStatus = gRT->GetVariable (BDS_CLOCKLOG_STATE_VARNAME, &gQcomTokenSpaceGuid,
             NULL, &VarSize, &Flag);

  if(eStatus != EFI_SUCCESS)
  {
    AsciiPrint (" BDS Menu: Clock log State setting  isn't initialized.\r\n");
    AsciiPrint (" >>> Select <CLOCK Menu> under BDS Menu  \r\n");
    AsciiPrint (" >>> Select <Toggle Logging Clocks states> \r\n");
    AsciiPrint (" >>> Select <1> to dump clock state to UART \r\n");
    return eStatus;
  }

  if (Flag == BDSClockLogStateDisable)
  {
    AsciiPrint ("BDS Menu: Clock log State is disable.\r\n");
    return eStatus;
  }

  eStatus = gBS->LocateProtocol(&gEfiTestClockProtocolGuid,NULL,
                        (VOID **)&pTestClockProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pTestClockProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Test Clock Protocol not available \r\n");
    return eStatus;
  }

  eStatus = pTestClockProtocol->DumpClocksState(pTestClockProtocol, 0);

  if (eStatus == EFI_SUCCESS)
    AsciiPrint ("BDS Menu: Clock log State dumping success.\r\n");
  else
    AsciiPrint ("BDS Menu: Unable to dump Clock log State\r\n");

  return eStatus;
}

#define  BDS_CPU_CLOCK_4_CLUSTERS_MAP_STRING     " 0 = Silver Cluster"    "\r\n" \
                                                 " 1 = Gold Cluster"      "\r\n" \
                                                 " 2 = Gold Plus Cluster" "\r\n" \
                                                 " 3 = L3"

#define  BDS_CPU_CLOCK_3_CLUSTERS_MAP_STRING     " 0 = Silver Cluster (Cpu 0)"    "\r\n" \
                                                 " 1 = Silver Cluster (Cpu 1)"    "\r\n" \
                                                 " 2 = Silver Cluster (Cpu 2)"    "\r\n" \
                                                 " 3 = Silver Cluster (Cpu 3)"    "\r\n" \
                                                 " 4 = Gold Cluster (Cpu 4)"      "\r\n" \
                                                 " 5 = Gold Cluster (Cpu 5)"      "\r\n" \
                                                 " 6 = Gold Cluster (Cpu 6)"      "\r\n" \
                                                 " 7 = Gold Cluster (Cpu 7)"      "\r\n" \
                                                 " 8 = L3 (Cpu 8)"

EFI_STATUS
DumpCPUClocksPlan (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS     eStatus;
  UINT32         PerfLevel;
  UINT32         CpuFreq;
  UINT32         Vg;
  UINT32         j;
  INT8           nCpu = 0;
  UINT8          MaxVal;
  UINTN          VarSize;
  EFI_CLOCK_PROTOCOL     *pClockProtocol;
  EFI_CHIPINFO_PROTOCOL  *pChipInfoProtocol;
  EFIChipInfoFamilyType   ChipFamily = EFICHIPINFO_FAMILY_UNKNOWN;
  CONST CHAR8            *ValueMapStr;

  eStatus = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL,
                               (VOID **)&pChipInfoProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pChipInfoProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get ChipInfo protocol\r\n");
    return eStatus;
  }

  pChipInfoProtocol->GetChipFamily(pChipInfoProtocol, &ChipFamily);

  VarSize = sizeof(nCpu);
  eStatus = gRT->GetVariable (BDS_CPU_CLOCK_CLUSTER_VARNAME, &gQcomTokenSpaceGuid,
             NULL, &VarSize, &nCpu);
  if(eStatus != EFI_SUCCESS)
    nCpu = 0;

  switch(ChipFamily)
  {
    case EFICHIPINFO_FAMILY_SDM855:
      MaxVal = BDS_HANA_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_4_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM1000:
      MaxVal = BDS_POIPU_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_3_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM640: /* TODO */
    default:
      AsciiPrint ("BDS Menu: Unable to get Chip family info\r\n");
      return eStatus;
  }
  if (QuerySetItem ("Dump CPU Clocks Frequency plan, select CPU Id", ValueMapStr, &nCpu, 0, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(nCpu);
  eStatus = gRT->SetVariable (BDS_CPU_CLOCK_CLUSTER_VARNAME, &gQcomTokenSpaceGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              VarSize, &nCpu);
  if(eStatus != EFI_SUCCESS)
  {
    AsciiPrint (" BDS Menu: Can't select Cpu Clock Cluster.\r\n");
    return eStatus;
  }

  eStatus = gBS->LocateProtocol(&gEfiClockProtocolGuid,NULL,
                        (VOID **)&pClockProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pClockProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get Clock protocol\r\n");
    return eStatus;
  }

  pClockProtocol->GetMaxPerfLevel (pClockProtocol, (UINT32)nCpu, &PerfLevel);
  AsciiPrint ("CPU Cluster %d Frequency plan \r\n", nCpu);
  AsciiPrint ("PerfLevel     Frequency(MHz)      Voltage(mv) \r\n");
  for (j = 0; j < PerfLevel; j++)
  {
    pClockProtocol->GetCpuPerfLevelFrequency (pClockProtocol, (UINT32)nCpu, j, &CpuFreq, &Vg);
    CpuFreq = CpuFreq / 1000000;
    AsciiPrint ("%8d      %8d     %8d\n", j, CpuFreq, Vg);
  }
  return eStatus;
}

EFI_STATUS
GetCPUClocksFrequency (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS    eStatus;
  INT8          nCpu = 0;
  UINT32         PerfLevel;
  UINT32         CpuFreq;
  UINT32         Vg;
  UINT8         MaxVal;
  UINTN         VarSize;
  EFI_CLOCK_PROTOCOL     *pClockProtocol;
  EFI_CHIPINFO_PROTOCOL  *pChipInfoProtocol;
  EFIChipInfoFamilyType   ChipFamily = EFICHIPINFO_FAMILY_UNKNOWN;
  CONST CHAR8            *ValueMapStr;

  eStatus = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL,
                               (VOID **)&pChipInfoProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pChipInfoProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get ChipInfo protocol\r\n");
    return eStatus;
  }

  pChipInfoProtocol->GetChipFamily(pChipInfoProtocol, &ChipFamily);


  VarSize = sizeof(nCpu);
  eStatus = gRT->GetVariable (BDS_CPU_CLOCK_CLUSTER_VARNAME, &gQcomTokenSpaceGuid,
             NULL, &VarSize, &nCpu);
  if(eStatus != EFI_SUCCESS)
    nCpu = 0;

  switch(ChipFamily)
  {
    case EFICHIPINFO_FAMILY_SDM855:
      MaxVal = BDS_HANA_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_4_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM1000:
      MaxVal = BDS_POIPU_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_3_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM640: /* TODO */
    default:
      AsciiPrint ("BDS Menu: Unable to get Chip family info\r\n");
      return eStatus;
  }

  if (QuerySetItem ("Get CPU Clocks Frequency, select cluster", ValueMapStr, &nCpu, 0, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(nCpu);
  eStatus = gRT->SetVariable (BDS_CPU_CLOCK_CLUSTER_VARNAME, &gQcomTokenSpaceGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              VarSize, &nCpu);
  if(eStatus != EFI_SUCCESS)
  {
    AsciiPrint (" BDS Menu: Can't get Cpu Clock Cluster.\r\n");
    return eStatus;
  }

  eStatus = gBS->LocateProtocol(&gEfiClockProtocolGuid,NULL,
                        (VOID **)&pClockProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pClockProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get Clock protocol\r\n");
    return eStatus;
  }

  eStatus = pClockProtocol->GetCpuPerfLevel (pClockProtocol, (UINT32)nCpu, (UINT32 *)&PerfLevel);
  if ((eStatus != EFI_SUCCESS) ||(PerfLevel == 0xFF))
  {
    CpuFreq = 0;
    Vg = 0;
  }
  else
  {
    pClockProtocol->GetCpuPerfLevelFrequency (pClockProtocol, (UINT32)nCpu, (UINT32)PerfLevel, (UINT32*)&CpuFreq, (UINT32*)&Vg);
  }
  CpuFreq = CpuFreq / 1000000;
  AsciiPrint ("BDS Menu: CPU clock ID %d frequency %d MHz, Voltage(mv) %d\r\n",nCpu, CpuFreq, Vg);
  return eStatus;
}

EFI_STATUS
SetCPUClocksFrequency (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS     eStatus;
  INT8           nCpu = 0;
  INT8           perfl = 0;
  UINTN          VarSize;
  UINT32         PerfLevel, CpuFreq;
  UINT8          MaxVal;
  EFI_CLOCK_PROTOCOL *pClockProtocol;
  EFI_CHIPINFO_PROTOCOL  *pChipInfoProtocol;
  EFIChipInfoFamilyType   ChipFamily = EFICHIPINFO_FAMILY_UNKNOWN;
  CONST CHAR8            *ValueMapStr;

  eStatus = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL,
                               (VOID **)&pChipInfoProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pChipInfoProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get ChipInfo protocol\r\n");
    return eStatus;
  }
  pChipInfoProtocol->GetChipFamily(pChipInfoProtocol, &ChipFamily);

  eStatus = gBS->LocateProtocol(&gEfiClockProtocolGuid,NULL,
                        (VOID **)&pClockProtocol );

  if ((eStatus != EFI_SUCCESS) ||(pClockProtocol == NULL))
  {
    AsciiPrint ("BDS Menu: Unable to get Clock protocol\r\n");
    return eStatus;
  }
  VarSize = sizeof(nCpu);
  eStatus = gRT->GetVariable (BDS_CLOCKLOG_STATE_VARNAME, &gQcomTokenSpaceGuid,
             NULL, &VarSize, &nCpu);
  if(eStatus != EFI_SUCCESS)
    nCpu = 0;

  switch(ChipFamily)
  {
    case EFICHIPINFO_FAMILY_SDM855:
      MaxVal = BDS_HANA_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_4_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM1000:
      MaxVal = BDS_POIPU_CPU_MAX;
      ValueMapStr = BDS_CPU_CLOCK_3_CLUSTERS_MAP_STRING;
      break;
    case EFICHIPINFO_FAMILY_SDM640: /* TODO */
    default:
      AsciiPrint ("BDS Menu: Unable to get Chip family info\r\n");
      return eStatus;
  }
  nCpu = 0;

  if (QuerySetItem ("Set CPU Clocks Frequency, Select Cpu ID", ValueMapStr, (INT8 *)&nCpu, 0, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;
  VarSize = sizeof(nCpu);
  eStatus = gRT->SetVariable (BDS_CPU_CLOCK_CLUSTER_VARNAME, &gQcomTokenSpaceGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              VarSize, &nCpu);

  pClockProtocol->GetMaxPerfLevel (pClockProtocol, (UINT32)nCpu, (UINT32 *)&PerfLevel);

  AsciiPrint ("Cpu ID %d  Max PerfLevel %d \r\n", nCpu, PerfLevel);

  perfl = 0;
  VarSize = sizeof(perfl);
  MaxVal = PerfLevel-1;
  if (QuerySetItem ("Select PrefLevel",NULL, &perfl, 0, MaxVal) != EFI_SUCCESS)
      return EFI_SUCCESS;

  eStatus = gRT->SetVariable (L"BDSCpuClockClusterPerfLevel", &gQcomTokenSpaceGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              VarSize, &perfl);

  if ((eStatus != EFI_SUCCESS)||(perfl > MaxVal))
  {
    AsciiPrint ("BDS Menu: Fail Set CPU ID %d PerfLevel %d.\r\n", nCpu, perfl);
    return eStatus;
  }
  PerfLevel = (UINT32)perfl;
  pClockProtocol->SetCpuPerfLevel (pClockProtocol, (UINT32)nCpu, PerfLevel, &CpuFreq);
  CpuFreq = CpuFreq / 1000000;
  AsciiPrint ("BDS Menu: Set CPU ID %d clk is %d MHz (PerfLevel %d) \r\n",nCpu, CpuFreq, PerfLevel);
  return eStatus;
}

/*
 * Launch ChargerTest App
 */

EFI_STATUS
ChargerTest (CHAR8* Arg)
{
  EFI_STATUS    Status = EFI_SUCCESS;

  /* Launch Charger Test App here with default argument as to test Ex Protocol APIs */
  //Status = LaunchAppFromGuidedFv(&gToolsFvGuid, "ChargerTest", NULL);
  if(Status == EFI_SUCCESS)
    AsciiPrint("Charger Test App Launch Successful \r\n");
  else
    AsciiPrint("ERROR: Launching Charger Test App %r \r\n", Status);

  return Status;
}


/*
 * Handler to Set UsbConfigPrimaryPort variable to modify
 * the configuration of the USB Primary cores.
 */
#define  USB_CONFIG_PRIMARY_PORT_STRING     " 1 = Host Mode"      "\r\n" \
                                            " 2 = Device mode (FFU, MassStorage)"     "\r\n" \
                                            " 3 = Dual Role (Based on type of cable)"

EFI_STATUS
SetUsbConfigPrimaryPort (CHAR8* Arg)
{
  EFI_STATUS    Status;
  INT8          Flag = 0;
  UINTN         VarSize;
  CONST UINT8   DefaultVal = 3;
  CONST UINT8   MinVal     = 1;
  CONST UINT8   MaxVal     = 3;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"UsbConfigPrimaryPort", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  //If the variable does not exit, use the default value
  if (Status != EFI_SUCCESS)
  {
    Flag = DefaultVal;
  }

  if (QuerySetFlag ("USB Configuration on Primary Port", USB_CONFIG_PRIMARY_PORT_STRING, &Flag, MinVal, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"UsbConfigPrimaryPort", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler to Set UsbConfigSecondaryPort variable to modify
 * the configuration of the USB Secondary cores.
 */
#define  USB_CONFIG_SECONDARY_PORT_STRING   " 1 = Host Mode"      "\r\n" \
                                            " 2 = Device mode (FFU, MassStorage)"     "\r\n" \
                                            " 3 = Dual Role (Based on type of cable)"

EFI_STATUS
SetUsbConfigSecondaryPort (CHAR8* Arg)
{
  EFI_STATUS    Status;
  INT8          Flag = 0;
  UINTN         VarSize;
  CONST UINT8   DefaultVal = 3;
  CONST UINT8   MinVal     = 1;
  CONST UINT8   MaxVal     = 3;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"UsbConfigSecondaryPort", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  //If the variable does not exit, use the default value
  if (Status != EFI_SUCCESS)
  {
    Flag = DefaultVal;
  }

  if (QuerySetFlag ("USB Configuration on Secondary Port", USB_CONFIG_SECONDARY_PORT_STRING, &Flag, MinVal, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"UsbConfigSecondaryPort", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}


/*
*
 * Handler to toggle the SetHsUartBaudRate variable to enable
 * high speed 3Mbps UART.
 */

EFI_STATUS
ToggleEnableHsUartBaudRate (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8          Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"SetHsUartBaudRate", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Set 3Mbps UART Baud Rate", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"SetHsUartBaudRate", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for ToggleHSUsbfnForceHS Command
 */
EFI_STATUS
ToggleHSUsbfnForceHS (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"UsbfnHijackRequests", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Force Usbfn HighSpeed Enumeration", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"UsbfnHijackRequests", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for SetUsbfnMaxSpeed Command
 */
#define  USB_MAX_SPEED_STRING     " 0 = HighSpeed"        "\r\n" \
                                  " 1 = FullSpeed"        "\r\n" \
                                  " 2 = Reserved"         "\r\n" \
                                  " 3 = Reserved"         "\r\n" \
                                  " 4 = SuperSpeed"       "\r\n" \
                                  " 5 = SuperSpeed-Plus"
EFI_STATUS
SetUsbfnMaxSpeed(CHAR8* Arg)
{
  EFI_STATUS    Status;
  INT8          MaxSpeed = 0;
  UINTN         VarSize;
  CONST UINT8   DefaultVal = 5;
  CONST UINT8   MinVal     = 0;
  CONST UINT8   MaxVal     = 5;

  VarSize = sizeof(MaxSpeed);
  Status = gRT->GetVariable (L"UsbfnMaxSpeed", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &MaxSpeed);

  //If the variable does not exist, use the default value
  if (Status != EFI_SUCCESS)
  {
    MaxSpeed = DefaultVal;
  }

  if (QuerySetFlag ("Usbfn Max Speed", USB_MAX_SPEED_STRING, &MaxSpeed, MinVal, MaxVal) != EFI_SUCCESS)
    return EFI_SUCCESS;

  switch (MaxSpeed) {
    case 0:
    case 1:
    case 4:
    case 5:
      break;

    default:
      MaxSpeed = 5;
      AsciiPrint("Using default max speed, SuperSuper-Plus \r\n");
      break;
  }

  VarSize = sizeof(MaxSpeed);
  Status = gRT->SetVariable (L"UsbfnMaxSpeed", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &MaxSpeed);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for ToggleForceUsbDeviceModeCompliance Command
 */

EFI_STATUS
ToggleForceUsbDeviceModeCompliance (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"UsbfnForceComplianceMode", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Force Usbfn Compliance Mode", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"UsbfnForceComplianceMode", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}


/*
 * Handler for ToggleForceQcUsbFnHlos Command for Primary Port only.
 */

EFI_STATUS
ToggleForceQcUsbFnHlos (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"UsbfnEnableQcFnHLOS", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Enable QcUsbFn HLOS driver", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"UsbfnEnableQcFnHLOS", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for ToggleEnableSLTLoopback Command.
 */

EFI_STATUS
ToggleEnableSLTLoopback (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableSLTLoopback", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Enable SLT Loopback Test", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableSLTLoopback", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler to set the delay time used when BootUsbFirst option
 * is enabled.
 */
EFI_STATUS
SetBootUsbFirstDelayValue(CHAR8 *Arg)
{
  EFI_STATUS    Status          = EFI_SUCCESS;
  INT8          Flag            = 0;
  UINTN         VarSize         = sizeof(Flag);
  CONST UINT8   DefaultDelayVal = 30;
  CONST UINT8   MinVal          = 0;
  CONST UINT8   MaxVal          = 50;
  CONST CHAR8   *FeatureString  = "Set BootUsbFirstDelay Value";
  CONST CHAR8   *ValueMapStr    = "X second delay";

  // Query BootUsbFirstDelayValue variable
  Status = gRT->GetVariable(L"BootUsbFirstDelayValue",
                            &gQcomTokenSpaceGuid,
                            NULL,
                            &VarSize,
                            &Flag);

  // if the variable does not exit, use the default value
  if (Status != EFI_SUCCESS)
  {
    Flag = DefaultDelayVal;
  }

  // allow the user to set the new delay value
  Status = QuerySetFlag(FeatureString,
                        ValueMapStr,
                        &Flag,
                        MinVal,
                        MaxVal);

  if (Status != EFI_SUCCESS)
  {
    return EFI_SUCCESS;
  }

  // store the change for the delay value
  VarSize = sizeof(Flag);
  Status = gRT->SetVariable(L"BootUsbFirstDelayValue",
                            &gQcomTokenSpaceGuid,
                            (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE),
                            VarSize,
                            &Flag);

  if (Status == EFI_SUCCESS)
  {
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  }
  else
  {
    AsciiPrint("ERROR: Unable to change/store setting\r\n");
  }

  return Status;
}

/*
 * Handler to Toggle Display During Charging
 */
EFI_STATUS
ToggleDisplayDuringCharging (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"DispCharging", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Toggle Display During Charging", &Flag, INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"DispCharging", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler to Toggle Enable Volatile Boot Options
 */
EFI_STATUS
ToggleEnableVolatileBootOptions (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableVolatileBootOptions", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Toggle Enable Volatile Boot Options", &Flag, FALSE) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableVolatileBootOptions", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler to Toggle EnableOnstorageDump
 */
EFI_STATUS
ToggleEnableOnstorageDump (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (ENABLE_ONSTORAGE_DUMP_VARNAME, &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Toggle EnableOnstorageDump", &Flag, FALSE) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (ENABLE_ONSTORAGE_DUMP_VARNAME, &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler to Toggle OfflineMemoryDumpUseCapability
 */
EFI_STATUS
ToggleOfflineMemoryDumpUseCapability (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;
  UINT8     FlagNew = 0;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (OFFLINE_MEMORY_DUMP_USE_CAPABILITY_VARNAME, &gOSAVendorGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  FlagNew = Flag&0x1;

  if (QueryToggleFlag ("Toggle OfflineMemoryDumpUseCapability", &FlagNew, FALSE) != EFI_SUCCESS)
    return EFI_SUCCESS;

  Flag = (Flag&0xFE)|FlagNew;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (OFFLINE_MEMORY_DUMP_USE_CAPABILITY_VARNAME, &gOSAVendorGuid,
                             OFFLINE_MEMORY_DUMP_USE_CAPABILITY_ATTR,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for EnableHSUsbPrimary Command for Primary Port only.
 */
EFI_STATUS
EnableHSUsbPrimary (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableHighSpeedOnlyForPri", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Force HighSpeed Enumeration for Primary Port", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableHighSpeedOnlyForPri", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for EnableHSUsbSecondary Command for Secondary Port only.
 */
EFI_STATUS
EnableHSUsbSecondary (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableHighSpeedOnlyForSec", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Force HighSpeed Enumeration for Secondary Port", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableHighSpeedOnlyForSec", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for EnableUsbDPBondedPrimary Command for Secondary Port only.
 */
EFI_STATUS
EnableUsbDPBondedPrimary (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableDPBondedForPri", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Skip GCC Reset for DP Bonded Mode for Primary Port", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableDPBondedForPri", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

/*
 * Handler for EnableUsbDPBondedSecondary Command for Secondary Port only.
 */
EFI_STATUS
EnableUsbDPBondedSecondary (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableDPBondedForSec", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS) {
    Flag = 0;
  }

  if (QueryToggleFlag ("Skip GCC Reset for DP Bonded Mode for Secondary Port", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"EnableDPBondedForSec", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

EFI_STATUS
XpuErrFatal(CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"XpuErrFatalEnable", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("XPU Error Fatal", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  Status = gRT->SetVariable (L"XpuErrFatalEnable", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Reboot/PowerCycle device gracefully for setting to take effect.\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;
}

VOID
TurnOnDisplay (VOID)
{
  EFI_STATUS  Status;
  EFI_DISPLAY_TYPE eDisplayType = EFI_DISPLAY_TYPE_PRIMARY;
  EFI_DISPLAY_POWER_CTRL_STATE ePowerState = EFI_DISPLAY_POWER_STATE_ON;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    DEBUG ((EFI_D_WARN, "Display Power Control protocol not available\r\n"));
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState);

  if (Status == EFI_SUCCESS)
    AsciiPrint("Successfully turned ON display\r\n");
  else
    AsciiPrint("Display turn ON Failed\r\n");
}

VOID
TurnOffDisplay (VOID)
{
  EFI_STATUS Status;
  EFI_DISPLAY_TYPE eDisplayType = EFI_DISPLAY_TYPE_PRIMARY;
  EFI_DISPLAY_POWER_CTRL_STATE ePowerState = EFI_DISPLAY_POWER_STATE_OFF;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    AsciiPrint("Display Power Control protocol not available\r\n");
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState);

  if (Status == EFI_SUCCESS)
    AsciiPrint("Successfully turned OFF display\r\n");
  else
    AsciiPrint("Display turne OFF Failed\r\n");
}

VOID
LowClock (VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_CLOCK_PROTOCOL  *mClockProtocol;

  Status = gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID **)&mClockProtocol);
  if ((Status != EFI_SUCCESS) || (mClockProtocol == NULL))
  {
    AsciiPrint("Clock protocol not available\r\n");
    return;
  }
  Status = mClockProtocol->EnterLowPowerMode(mClockProtocol);

  if (Status == EFI_SUCCESS)
    AsciiPrint("Switched to low clock.\r\n");
  else
    AsciiPrint("Switch to low clock FAILED.\r\n");
}

VOID
HighClock (VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_CLOCK_PROTOCOL  *mClockProtocol;

  Status = gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID **)&mClockProtocol);
  if ((Status != EFI_SUCCESS) || (mClockProtocol == NULL))
  {
    AsciiPrint("Clock protocol not available\r\n");
    return;
  }

  Status = mClockProtocol->ExitLowPowerMode(mClockProtocol);

  if (Status == EFI_SUCCESS)
    AsciiPrint("Switched to normal clock.\r\n");
  else
    AsciiPrint("Switch to normal clock FAILED.\r\n");
}

/*
 *  Low Power Mode
 */
EFI_STATUS
LowPowerMode (CHAR8* Arg)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN Disp, Clk, Opr;

  ASSERT_EFI_ERROR(Status);

  /* LowPowerMode xxxx
   * Where x can be one of the following (combine these logically)
   * D : Display
   * C : Clock
   * H : Restore all to normal power mode
   * R : Same as H
   * L : Low power mode (default if not specified, if no additional option should result into max savings)
   *
   * Example : LD => turn off display
   *           HD => Turn ON display
   *           LCK => Display off + Low clock (currently this is same as no LCK specified)
   *
   * */
#define  OPERATION_NONE        0
#define  OPERATION_LOW_PWR     1
#define  OPERATION_RESTORE     2
#define  OPERATION_LOWEST_PWR  3
#define  OPERATION_NORMAL_PWR  4

  Disp = 0;
  Clk = 0;
  Opr = OPERATION_NONE;

  if (Arg == NULL)
  {
    Opr = OPERATION_LOWEST_PWR;
  }
  else
  {
    CHAR8* ch = Arg;
    UINTN DevCnt = 0;

    while (ch && *ch)
    {
      switch (*ch)
      {
        case 'c': case 'C':
          Clk = 1;
          DevCnt++;
          break;

        case 'd': case 'D':
          Disp = 1;
          DevCnt++;
          break;

        case 'r': case 'R':
        case 'h': case 'H':
          Opr = OPERATION_RESTORE;
          break;

        case 'l': case 'L':
          Opr = OPERATION_LOW_PWR;
          break;

        default:
          ch = NULL;
      }
      if (ch)
        ++ch;
    }

    if (DevCnt == 0)
    {
      if (Opr == OPERATION_RESTORE)
        Opr = OPERATION_NORMAL_PWR;
      else if (Opr == OPERATION_LOW_PWR)
        Opr = OPERATION_LOWEST_PWR;
    }
  }

  switch (Opr)
  {
    case OPERATION_LOWEST_PWR:
      LowClock ();
      TurnOffDisplay ();
      break;

    case OPERATION_NORMAL_PWR:
      TurnOnDisplay ();
      HighClock ();
      break;

    case OPERATION_LOW_PWR:
      if (Clk)
        LowClock ();
      if (Disp)
        TurnOffDisplay ();
      break;

    case OPERATION_RESTORE:
      if (Disp)
        TurnOnDisplay ();
      if (Clk)
        HighClock ();
      break;
  }

  return Status;
}

EFI_STATUS
PrintChargerAppDbgMsg (CHAR8* Arg)
{
  EFI_STATUS    Status;
  UINT8         Flag = 0;
  UINTN         VarSize;

  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"PrintChargerAppDbgMsg", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if(Status != EFI_SUCCESS)
    Flag = 0;

  if (QueryToggleFlag ("Print Charger App Debug messages", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);
  Status = gRT->SetVariable (L"PrintChargerAppDbgMsg", &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize, &Flag);

  if(Status == EFI_SUCCESS)
    AsciiPrint("Successfully stored setting\r\n");
  else
    AsciiPrint("ERROR: Unable to change/store setting\r\n");

  return Status;

}


EFI_STATUS
EnPmicShipMode ()
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS Status;
  UINT8 VarValue = 0;
  UINT8 PmShip = 0;
  UINTN VarSize = 0;
  VarSize = sizeof(VarValue);

  Status = gRT->GetVariable (L"ShipMode", &gQcomTokenSpaceGuid,
             NULL, &VarSize, &VarValue);

  if (Status == EFI_NOT_FOUND)
    VarValue = 0;
  else
    // bit 1 of ShipMode NV represents if PMIC ship mode is enabled or not
    PmShip = VarValue & 0x2;

  if (QueryToggleFlag ("Toggle PMIC Ship Mode", &PmShip, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(VarValue);

  VarValue = PmShip << 1;
  Status = gRT->SetVariable (L"ShipMode", &gQcomTokenSpaceGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS,
             VarSize, &VarValue);


  if(Status == EFI_SUCCESS){
    AsciiPrint("PMIC Ship Mode ");
    if(PmShip == 0)
      AsciiPrint("Disabled. \r\n");
    else
      AsciiPrint("Enabled. \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }
  return Status;
}



EFI_STATUS
FileLoggingToggle (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS Status;
  UINT8 Flag = 0;
  UINTN VarSize = 0;

  VarSize = sizeof(Flag);

  Status = gRT->GetVariable (L"EnableFileLogging", &gQcomTokenSpaceGuid,
             NULL, &VarSize, &Flag);

  if (Status == EFI_NOT_FOUND)
    Flag = 0;

  if (QueryToggleFlag ("Toggle UEFI File Logging", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);

  Status = gRT->SetVariable (L"EnableFileLogging", &gQcomTokenSpaceGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             VarSize, &Flag);

  if(Status == EFI_SUCCESS){
    AsciiPrint("File Logging has been ");
    if(Flag == 0)
      AsciiPrint("disabled. \r\n");
    else
      AsciiPrint("enabled. \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }
  return Status;
}



BOOLEAN
CheckValid(EFI_DEVICE_PATH_PROTOCOL *DevPath)
{
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_BLOCK_IO_PROTOCOL     *BlockIo;
  EFI_HANDLE                Handle;
  EFI_STATUS Status;

  TempDevicePath = DevPath;
  Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &TempDevicePath, &Handle);

  if (!EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlockIo);
    if (!EFI_ERROR (Status))
      return TRUE;
  } else {
    Status = gBS->LocateDevicePath (&gEfiSimpleFileSystemProtocolGuid, &TempDevicePath, &Handle);
    if (!EFI_ERROR (Status))
      return TRUE;
  }

  return FALSE;
}




VOID
Add64BitBootOrder(CHAR8* Arg)
{
  EFI_STATUS                 Status;
  UINTN                      HandleCount;
  EFI_HANDLE                 *HandleBuffer;
  UINTN                      Index;
  EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  EFI_BLOCK_IO_PROTOCOL      *BlkIo;
  UINTN AddFlag = 0;

  DEBUG((EFI_D_ERROR, "Adding all non-removable options to BootOrder\n"));


  Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiSimpleFileSystemProtocolGuid,
                      NULL,
                      &HandleCount,
                      &HandleBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Could not locate by FS protocol\n"));
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiBlockIoProtocolGuid,
                      (VOID **) &BlkIo
                      );

    if (BlkIo == NULL)
      continue;

    if (BlkIo->Media->RemovableMedia)
      continue;

    Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiDevicePathProtocolGuid,
                      (VOID **) &DevicePath
                      );

    if (DevicePath == NULL) {
      DEBUG((EFI_D_ERROR, "Could not get device path\n"));
      continue;
    }
    if (CheckValid(DevicePath))
    {
      AddFlag = ADD_FLAG_ADD_FIRST | ADD_FLAG_NO_BOOT_NEXT;

      Status = AddDevPathToBootOrder(NULL,
                                     DevicePath,
                                     L"Testing BootOrder",
                                     NULL,
                                     0,
                                     AddFlag);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "Error adding option to BootOrder\n"));
        continue;
      }

      DEBUG((EFI_D_ERROR, "Option found, adding to BootOrder\n"));
    }
  }
}


/*
 * Handler to command for mounting a FV
 */
EFI_STATUS
MountFvCommand (UINTN  Argc, CHAR8  **Argv)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GUID PartitionGuid;
  CHAR8    GuidStr[GUID_STRING_LENGTH+1];
  CHAR8*    Ch;

  switch (Argc)
  {
    case 1:
        Status = MountFvPartition(NULL, Argv[1]);
        break;

    case 2:
        if ((AsciiStrnCmp(Argv[1], "-g", 2)) == 0)
        {
            AsciiStrnCpy(GuidStr, Argv[2], GUID_STRING_LENGTH+1);
            Ch = &GuidStr[0];
            while (*Ch != '\0') {
                if (*Ch == '-')
                    *Ch = ' ';
                Ch++;
            }
            HexStrToBin ((UINT8*)GuidStr, AsciiStrLen((CHAR8*)GuidStr),
                      (UINT32*)&PartitionGuid, sizeof(EFI_GUID));
            ByteStreamToGUID((UINT8*)&PartitionGuid);
            Status = MountFvPartition(&PartitionGuid, NULL);
        }
        else
            Status = EFI_INVALID_PARAMETER;
        break;

    default:
        AsciiPrint("Error: MountFv requires Fv partition name or Guid.\n");
        AsciiPrint("eg: MountFv toolsfv.\n");
        AsciiPrint("eg: MountFv -g 11111111-1111-1111-1111-111111111111.\r\n");
        Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

/*
 * Force enable HW wdog, variable read by WatchDogDxe
 */
EFI_STATUS
ToggleHWWdog(VOID)
{
  EFI_STATUS Status;
  UINT8 Flag = 0;
  UINTN VarSize = 0;

  VarSize = sizeof(Flag);

  Status = gRT->GetVariable (L"ForceEnableHWWdog", &gQcomTokenSpaceGuid,
             NULL, &VarSize, &Flag);

  if (Status == EFI_NOT_FOUND)
    Flag = 0;

  if (QueryToggleFlag ("Force Enable HW WatchDog", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(Flag);

  Status = gRT->SetVariable (L"ForceEnableHWWdog", &gQcomTokenSpaceGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             VarSize, &Flag);

  if (Status == EFI_SUCCESS)
  {
    AsciiPrint ("ForceEnableHWWdog is : ");

    if (Flag == 0)
      AsciiPrint ("Disabled. \r\n");
    else
      AsciiPrint ("Enabled. \r\n");
  }
  else
  {
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }

  return Status;
}

/*
 * Toggle DISABLEBATTERY UEFI variable
 */
EFI_STATUS
ToggleDISABLEBATTERY (CHAR8* Arg)
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS Status;
  UINT8 Flag = 0;
  UINTN VarSize = 0;
  BOOLEAN bVariablePresent = FALSE;

  VarSize = sizeof(Flag);

  Status = gRT->GetVariable (L"DISABLEBATTERY", &gOSAVendorGuid,
             NULL, &VarSize, &Flag);

  if (Status == EFI_NOT_FOUND)
  {
    Flag = 0;
    bVariablePresent = FALSE;
  }
  else
  {
    bVariablePresent = TRUE;
  }

  if (QueryToggleFlag ("Toggle UEFI Variable: DISABLEBATTERY", &Flag, NON_INVERTED_MEANING) != EFI_SUCCESS)
  return EFI_SUCCESS;

  if (Flag != 0 && bVariablePresent == FALSE)
  {
    //Request Set the UEFI Variable and the flag is currently not exist
    VarSize = sizeof(Flag);
  }

  Status = gRT->SetVariable (L"DISABLEBATTERY", &gOSAVendorGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                             VarSize,
                             &Flag);

  if (Status == EFI_SUCCESS)
  {
    AsciiPrint ("UEFI Variable: DISABLEBATTERY ");
    if (Flag == 0)
      AsciiPrint("Disable(Deleted). \r\n");
    else
      AsciiPrint("Enabled(Set). \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }

  return Status;
}

EFI_STATUS
EnFgShipMode()
{
    // create the variable if it does not exist
    EFI_STATUS Status;
    UINT8 VarValue = 0;
    UINTN VarSize = 0;
    UINT8 FgShip = 0;

    VarSize = sizeof(VarValue);

    Status = gRT->GetVariable (L"ShipMode", &gQcomTokenSpaceGuid,
               NULL, &VarSize, &VarValue);

    if (Status == EFI_NOT_FOUND)
      VarValue = 0;
  else
    // bit 0 of ShipMode NV represents whether FG ship mode is enabled or not
    FgShip = VarValue & 0x1;

    if (QueryToggleFlag ("Toggle FG Ship Mode", &FgShip, NON_INVERTED_MEANING) != EFI_SUCCESS)
      return EFI_SUCCESS;

    VarSize = sizeof(VarValue);

    Status = gRT->SetVariable (L"ShipMode", &gQcomTokenSpaceGuid,
                 EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS,
                 VarSize, &FgShip);

  if (Status == EFI_SUCCESS)
  {
      AsciiPrint("FG Ship Mode ");
    if (FgShip == 0)
      AsciiPrint("Disable(Deleted). \r\n");
    else
      AsciiPrint("Enabled(Set). \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }

  return Status;

}

/*
 * Toggle UEFI variable to verify FW Update feature
 */
EFI_STATUS
TestFwUpdateSupport (VOID)
{
  EFI_STATUS Status;
  UINT8 Flag = 0;
  UINTN VarSize = 0;
  UINT8 TestFwUpdate = 0;

  VarSize = sizeof(Flag);

  Status = gRT->GetVariable (L"TestFwUpdateSupport", &gOSAVendorGuid,
                NULL, &VarSize, &Flag);

  if (Status == EFI_NOT_FOUND)
  {
    Flag = 0;
  }
  else
  {
    TestFwUpdate = Flag & 0x1;
  }

  if (QueryToggleFlag ("FW Update feature verification", &TestFwUpdate, NON_INVERTED_MEANING) != EFI_SUCCESS)
    return EFI_SUCCESS;

  VarSize = sizeof(TestFwUpdate);

  Status = gRT->SetVariable (L"TestFwUpdateSupport", &gOSAVendorGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                VarSize,
                &TestFwUpdate);

  if (Status == EFI_SUCCESS)
  {
    AsciiPrint ("UEFI Variable: TestFwUpdateSupport : ");

    if (TestFwUpdate == 0)
      AsciiPrint("Disable (Deleted)\r\n");
    else
    {
      AsciiPrint("Enabled (Set)\r\n");
      AsciiPrint("\tRun ChargerTest to verify FW Update feature\r\n\n");
    }
  }
  else
  {
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }

  return Status;
}

/*
 * Trigger a non-secure WDOG bite
 */
EFI_STATUS
TriggerWdogBite(VOID)
{
  EFI_STATUS Status;
  EFI_QCOM_WATCHDOG_PROTOCOL  *WdogProt;

  Status = gBS->LocateProtocol (
          &gEfiQcomWDogProtocolGuid,
          NULL,
          (VOID **) &WdogProt);

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_WARN, "Failed to locate Wdog protocol %r\n", Status));
    return EFI_NOT_FOUND;
  }

  WdogProt->Enable();
  WdogProt->ForceWDogBite();
  return EFI_SUCCESS;
}
EFI_STATUS
EnOffModeSrc()
{
    // create the variable if it does not exist
    EFI_STATUS Status;
    UINT8 VarValue = 0;    
    UINT8 Mask = 0x2;
    UINTN VarSize = 0;
    UINT8 OffModeSrc = 0;

    VarSize = sizeof(VarValue);

    Status = gRT->GetVariable (L"OffModeSrc", &gQcomTokenSpaceGuid,
               NULL, &VarSize, &VarValue);

    if (Status == EFI_NOT_FOUND)
      VarValue = 0;
    else
      // bit 1 of OffModeSrc NV represents whether Off Mode Src feature is enabled or not
      OffModeSrc = (VarValue & Mask) >> 1;

    if (QueryToggleFlag ("Toggle Off Mode Sourcing", &OffModeSrc, NON_INVERTED_MEANING) != EFI_SUCCESS)
      return EFI_SUCCESS;

    VarSize = sizeof(VarValue);

    if(OffModeSrc == 0)
      // ignore ChargeInBattery bit if OffModeSrc = 0
      VarValue = 0;
    else
      VarValue = (VarValue & ~Mask) | Mask;
    

    Status = gRT->SetVariable (L"OffModeSrc", &gQcomTokenSpaceGuid,
                 EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                 VarSize, &VarValue);

  if (Status == EFI_SUCCESS)
  {
      AsciiPrint("Off Mode Sourcing ");
    if (OffModeSrc == 0)
      AsciiPrint("Disable(Deleted). \r\n");
    else
      AsciiPrint("Enabled(Set). \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }

  return Status;

}

EFI_STATUS
EnHWAutoOffModeSrc()
{
  // check the variable's current value if it exists, and create it if not.
  EFI_STATUS Status;
  UINT8 VarValue = 0;
  UINT8 HwAutoMask = 0x1;
  UINT8 OffModeSrcMask = 0x2;
  UINT8 HwAuto = 0;
  UINTN VarSize = 0;
  VarSize = sizeof(VarValue);

  Status = gRT->GetVariable (L"OffModeSrc", &gQcomTokenSpaceGuid,
             NULL, &VarSize, &VarValue);

  if (Status == EFI_NOT_FOUND)
    VarValue = 0;
  else
    // bit 0 of OffModeSrc NV represents if HwAutoOffModeSrc is enabled or not
    HwAuto = VarValue & HwAutoMask;

  if (VarValue & OffModeSrcMask)
  {
    // only allow toggling HwAuto when OffModeSrc is set
    if (QueryToggleFlag ("Toggle HW Auto mode for Off Mode Sourcing", &HwAuto, NON_INVERTED_MEANING) != EFI_SUCCESS)
      return EFI_SUCCESS;
    
    VarValue = (VarValue & ~HwAutoMask) | (HwAuto & HwAutoMask);
  }
  else
  {    
    // ignore HWAuto bit if OffModeSrc = 0
    VarValue = 0;
    HwAuto = 0;   
    AsciiPrint("HW Auto mode not configurable due to Off Mode Sourcing disabled\r\n");
  }

  VarSize = sizeof(VarValue);
  Status = gRT->SetVariable (L"OffModeSrc", &gQcomTokenSpaceGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             VarSize, &VarValue);


  if(Status == EFI_SUCCESS){
    AsciiPrint("HW Auto mode in Off Mode Sourcing ");
    if(HwAuto == 0)
      AsciiPrint("Disabled. \r\n");
    else
      AsciiPrint("Enabled. \r\n");
  }
  else{
    AsciiPrint("ERROR: Unable to store setting\r\n");
  }
  return Status;
}

