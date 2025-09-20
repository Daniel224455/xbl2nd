/** @file
  This file include all platform action which can be customized by IBV/OEM.

  Copyright (c) 2010-2020, 2022 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  Portions copyright (c) 2004 - 2008, Intel Corporation. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/22/19   kpa     Move MountRequiredFatPartitions before process capsules
 07/08/19   kpa     Renamed Api's, removed EnableOEMSetupAppInRetail
                    removed VariableServicesProtocol's SendMileStoneCall
 05/28/19   pa      Adding separate APi for capsule update
 05/08/19   bd      Skip logs flush to LOGFS partition if in Offmodecharging and on NVME device
 04/29/19   pj      Added log to measure HandleMorPpi time
 03/26/19   kpa     Restructured PlatformBdsInitEx into more granular api's, cleanup
 03/26/19   am      print platform subtype and flash type in DisplayPlatformInfo()
 03/18/19   kpa     remove BdsConnectAllDrivers
 03/15/19   kpa     remove redundant log
 02/05/19   kpa     Add support for deleting secure boot variables
 01/21/19   kpa     Reorganize PlatformBdsInitEx api, code cleanup
 12/09/18   kpa     relocate PlatformBdsDetectBootHotKey to QcomBds lib
 10/25/18   yw      print chip serial number in DisplayPlatformInfo()
 09/28/18   kpa     Added KernelBaseAddr, KernelSize variables
 09/12/18   md      QcomchargerApp Loading optimization
 09/11/18   kpa     Uart log cleanup
 09/11/18   kpa     Temporarily move connect all drivers at very end for nvme boot
 14/07/18   prema   Changes to move to FMP capsule processing
 06/12/18   pr      Removed SetHdcp() from Encryption protocol
 04/05/18   kpa     Get shared imem boot config value
 03/21/18   kpa     EFIESP load using GUID support
 02/16/18   bd      Set BootConfig Variable
 12/01/17   jt      Add NVMe support 
 08/17/17   aus     Added support to share SDCC register address 
 09/28/17   bd      Use cpu clock to initialize PMU cycle counter
 05/05/17   sj      Added the 64 bit cycle initialization at BDS stage
 10/31/16   bh      Remove splash screen rendering from BDS
 10/24/16   ai      Send SD card event when entering BDS menu
 08/26/16   vk      Remove unused function
 08/22/16   jt      Remove Sdcc config
 08/11/16   akl     Remove handling of NPA ExitBootServices. Handled in NpaDxe.
 03/30/16   ab      Moving handling of Glink ExitBootServices to GlinkDxe
 08/16/16   vk      Reduce logs in exit boot service for normal boot
 07/05/16   bh      Use partition label for file logging
 06/22/16   bh      Display total UEFI exec LA, move tools FV mounting
 06/22/16   bh      Updated retail checks
 06/08/16   sj      removed the key mapping for massstorage mode
 05/09/16   vk      Try to load EBL from test fv
 03/29/16   bh      Added logic to launch app from Guided Fv
 03/16/16   ml      Added rendering of the splash logo from within the BDS
 03/14/16   yg      Cleanup and remove dead code
 03/04/16   bh      Print UEFI execution time from start to POST
 02/22/16   vk      Load LinuxLoader from ABL
 02/22/16   vk      Add Shared_IMEM_Base variable
 01/27/16   sm      Replaced FgExit() API with FgBasicExit()
 01/11/16   ts      Removed PCIe init from BDS ExitBootServices
 10/15/15   bh      Simplify debug scripts to use info block
 10/14/15   vk      Added NULL check
 09/30/15   bh      Update firmware versioning
 07/07/11   niting  Initial revision.

=============================================================================*/

#include <PiDxe.h>

#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/EfiFileLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/UefiSigDb.h>
#include <Library/FuseControlLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/SecBootSigLib.h>

#include <Library/QcomBdsLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/HWConfigLib.h>
#include <Library/ProcAsmLib.h>

#include <Library/UefiCfgLib.h>
#include <Include/UefiInfoBlk.h>

#include <Library/KernelLib.h>
#include <Guid/Gpt.h>
#include <Guid/EventGroup.h>
#include <Guid/GlobalVariable.h>

#include <Library/FwUpdateLib.h>
#include <Library/FwProvisionLib.h>
#include <Library/GuidedFvLib.h>
#include <Library/BootConfig.h>
#include <Library/CapsuleLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadFile.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>

#include <Protocol/EFIVariableServices.h>
#include <Protocol/EFICapsule.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIPmicFg.h>
#include <Protocol/EFIToolSupport.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIHWIO.h>
#include <Protocol/EFIBootConfig.h>
#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIChargerEx.h>
#include <Protocol/VariablePolicy.h>

#include <Library/TimerLib.h>
#include <Library/LockLib.h>
#include <Library/QcomPerf.h>
#include <string.h>

#include "PlatformBds.h"

extern VOID BdsBootDeviceSelect ( VOID );
extern EFI_STATUS BdsWaitForSingleEvent (IN UINT64 Timeout);

//Partition Guid
extern EFI_GUID gEfiEmmcGppPartition1Guid;
extern EFI_GUID gEfiEmmcUserPartitionGuid;
extern EFI_GUID gEfiEmmcBootPartition1Guid;
extern EFI_GUID gEfiLogFSPartitionGuid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gToolsFvGuid;
extern EFI_GUID gMainFvGuid;

//Partition Type
extern EFI_GUID gEfiPartTypeSystemPartGuid;
extern EFI_GUID gEfiPlatPartitionTypeGuid;

extern EFI_GUID gEfiSdRemovableGuid;
extern EFI_GUID gEfiACPITableLoadGuid;
extern EFI_GUID gEfiAblFvNameGuid;

//Exit Boot Services guid
extern EFI_GUID gEfiEventExitBootServicesGuid;

//SCMDxe guid
extern EFI_GUID gQcomScmProtocolGuid;

//Charging Event GUID
extern EFI_GUID gEfiEventChargerEnableGuid;
extern EFI_GUID gEfiEventChargerDisableGuid;
extern EFI_GUID gEfiStartTimeHobGuid;
EFI_EVENT EventChargingStarted = NULL;
EFI_EVENT EventChargingEnd = NULL;
EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

EFI_EVENT ACPITableLoadEvent  = NULL;
EFI_EVENT EfiExitBootServicesEvent = (EFI_EVENT)NULL;
EFI_VARIABLESERVICES_PROTOCOL *VariableServicesProtocol = NULL;
QCOM_SCM_PROTOCOL *pSCMProtocol = NULL;
EFI_CLOCK_PROTOCOL *pClockProtocol = NULL;

STATIC UINT32 LcdDebugFlag = 0;
STATIC UINT8  EnableShellFlag = 0;
STATIC UINT8  VolatileTables = 0;
STATIC UINT8  EnableVolatileBootOptions = 0;

STATIC UINT32 BootCycleCount = 0;

STATIC BOOLEAN PlatSecurityEnabled  = FALSE;

/*
 * Platform Bds Lib State operations
 * Note:
 * Not all states need to be necessarily consumed.
 * Used to indicate sequence and manage dependencies
 */
typedef enum _PlatBdsStateType
{
   PLAT_BDS_DEFAULT       = 0x0,
   PRE_CONSOLE_INIT_DONE  = 0x1,
   POST_CONSOLE_INIT      = 0x2,
   PROCESS_HOT_KEYS       = 0x3,
   PLATFORM_SECURITY_SET  = 0x4,
   PROCESS_FW_CAPSULES    = 0x5,
   BDS_SOC_POST_PROC      = 0x6,
   POST_FW_CONFIG_SEC     = 0x7,   
   PLATBDS_STATE_MAX = (int)0xFFFFFFFFULL,
} PlatBdsStateType;

STATIC PlatBdsStateType PlatBdsState = PLAT_BDS_DEFAULT;

VOID
EFIAPI
PlatformBdsLoadShellForNonProdMode (IN  BDS_COMMON_OPTION *Option);

EFI_STATUS
EFIAPI
PlatformBdsPreLoadBootOption (IN  BDS_COMMON_OPTION *Option);

EFI_STATUS
EFIAPI
WriteLogBufToPartition (VOID);

STATIC
EFI_STATUS
DisplayBootTime (CHAR8* Key, CHAR8* Event, BOOLEAN OnLCD);

STATIC
VOID
DisplayUefiTotalTime (VOID);

/* Initialize the cycle counter to track performance */
STATIC VOID
StartCyclCounter (VOID);


/* Initialize the cycle counter to track performance */
STATIC VOID
StartCyclCounter (VOID)
{
  UINTN RegVal;
  UINT64 Val;
  UINT64 AppsProcClkMHz = 0;
  UINT32 FreqHz = 0;
  UINT32 PerfLevel;
  UINT32 VoltageMv;

  /* User mode enable to read in non secure mode */
  WriteUserEnReg (1);

  /* Reset counters */
  RegVal = (0x41 << 24) |  /* IMP */
           (4 << 11)    |  /* N */
           (1 << 2);       /* Reset CCNT */
  WritePMonCtlReg (RegVal);

  ReadCycleCntReg();

  if(gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL,
                                (VOID**) &pClockProtocol) == EFI_SUCCESS )
  {
    if(pClockProtocol->GetCpuPerfLevel(pClockProtocol, 0, &PerfLevel) == EFI_SUCCESS)
    {
      if(pClockProtocol->GetCpuPerfLevelFrequency(pClockProtocol, 0, PerfLevel,
                                            &FreqHz, &VoltageMv) == EFI_SUCCESS)
      {
        AppsProcClkMHz = FreqHz/(1000*1000); // Above API returns in Hz
      }
    }
  }
  
  if(AppsProcClkMHz == 0)
    DEBUG ((EFI_D_ERROR, "AppsProcClkMHz is zero \r\n")); 

  /*
   Use ARM generic timer as reference.
   Get time since boot in nano-second.  
   Convert it into PMU counts
   
   For 32-bit MSFT HAL requires 1:64 (i.e. counter increments every 64 clks)
   For 64-bit MSFT HAL requires 1:1
  */
  Val = GetTimeInNanoSecond (GetPerformanceCounter ());
  Val = Val * AppsProcClkMHz/1000;

  WriteCycleCntReg(Val);

  /* Check if write went through */
  ReadCycleCntReg();

  /* Enable Cycle counter */
  WriteCntEnSetReg (((UINT32)1 << 31));

  /* Check if we start counting */
  ReadCycleCntReg();

  /* Enable CCNT */
  RegVal = (0x41 << 24) |  /* IMP */
           (4 << 11)    |  /* N */
           (1);            /* Enable all counters */
  WritePMonCtlReg (RegVal);

  /* Disable User mode access */
  WriteUserEnReg (0);

  /* Write to TPIDRURW */
  WriteTPIDRURWReg (0x56430000);

  /* Write to TPIDRURO */
  WriteTPIDRUROReg (0);

  /* Example to Read the counter value, Should read small */
  ReadCycleCntReg();
}


/**
  Displays POST Time.

**/
VOID
EFIAPI
DisplayPOSTTime (VOID)
{
  DisplayUefiTotalTime ();
  DisplayBootTime("POST Time     ", "OS Loader", TRUE);
}

/**
 * Write protection call for the GPP and Boot partitions
 *@param  Event                 Event whose notification
 *                              function is being invoked.
 *
 *@param  Context               The pointer to the notification
 *                              function's context which is
 *                              implementation-dependent.
 */
VOID
EFIAPI
PlatformBDSExitBSCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

#ifndef PRE_SIL
  /* Dump Log Buffer to File */
  Status = WriteLogBufToPartition ();
  if (EFI_ERROR(Status))
  {
     DEBUG ((EFI_D_ERROR, "BDS: LogFs sync skipped, %r\r\n", Status));
  }
#endif

  Status = gBS->LocateProtocol (&gEfiVariableServicesProtocolGuid,
                                NULL,
                                (VOID**) &VariableServicesProtocol);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_INFO, "BDS: Variable services protocol %r\n", Status));
  }

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid,
                                NULL,
                                (VOID**) &pSCMProtocol);
  if (pSCMProtocol == NULL)
  {
    DEBUG ((EFI_D_WARN, "Could not locate pSCM protocol\n"));
  }

  /* Flush the NV Storage tables before turning ON write protection
   * Do this only if the protocol available. During initial stages the
   * the protocol might not be installed */
  if (VariableServicesProtocol != NULL)
  {
    UINT32 SyncEndTime, SyncStartTime = GetTimerCountms ();

    Status = VariableServicesProtocol->FlushVariableNV (VariableServicesProtocol);
    if (EFI_ERROR(Status))
    {
      DEBUG ((EFI_D_ERROR, "Failed to flush NV var, Status = 0x%08x\r\n", Status));
    }
    else
    {
      SyncEndTime = GetTimerCountms ();
      DEBUG ((EFI_D_WARN, "Sync Duration = %d ms\r\n", SyncEndTime - SyncStartTime));
    }
  }

  /* This is added after the flushing of NV vars as this callback will deregister the Listeners,
   * hence FLushing to RPMB/GPT will fail after this point until HLOS SCM driver (only RPMB is supported) is loaded.
   */
  if (pSCMProtocol != NULL)
  {
    Status = pSCMProtocol->ScmExitBootServicesHandler (pSCMProtocol);
  }

}


VOID
SetOsIndicationsSupported (VOID)
{
  EFI_STATUS Status;
  UINT64     OsIndicationSupport;
  UINTN      DataSize;
  UINT32     Attributes;

  DataSize = sizeof(OsIndicationSupport);
  Status = gRT->GetVariable (
                  L"OsIndicationsSupported",
                  &gEfiGlobalVariableGuid,
                  &Attributes,
                  &DataSize,
                  &OsIndicationSupport
                  );
  if (EFI_ERROR(Status)) {   
    DEBUG ((EFI_D_INFO, "OsIndicationsSupported variable not present \n"));  
    OsIndicationSupport = EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
  }
  else {
    DEBUG ((EFI_D_INFO, "OsIndicationsSupported variable present \n"));  
    // If the variable already exists then OR the new value
    OsIndicationSupport |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
  }
  
  Status = gRT->SetVariable (
                  L"OsIndicationsSupported",
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof(OsIndicationSupport),
                  &OsIndicationSupport
                  );
}

BOOLEAN
CheckBootToFWUIField (VOID)
{
  EFI_STATUS Status;
  UINT64     OsIndication = 0;
  UINTN      DataSize;
  UINT32     Attributes; 
  BOOLEAN    RetVal = FALSE;
  //
  // If OsIndications is invalid, remove it.
  // Invalid case
  //   1. Data size != UINT64  
  //   2. OsIndication attribute inconsistence
  //
  OsIndication = 0;
  Attributes = 0;
  DataSize = sizeof(OsIndication);
  Status = gRT->GetVariable (
                  L"OsIndications",
                  &gEfiGlobalVariableGuid,
                  &Attributes,
                  &DataSize,
                  &OsIndication
                  );

  if (!EFI_ERROR(Status)) {
    if (DataSize != sizeof(OsIndication) ||
        Attributes != (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)){

      DEBUG ((EFI_D_ERROR, "Unformalized OsIndications variable exists. Delete it\n"));
      Status = gRT->SetVariable (
                      L"OsIndications",
                      &gEfiGlobalVariableGuid,
                      Attributes,
                      0,
                      &OsIndication
                      );
      ASSERT_EFI_ERROR (Status);
      
      RetVal = FALSE;
      }
    else {    
      if(OsIndication & EFI_OS_INDICATIONS_BOOT_TO_FW_UI) {
        DEBUG ((EFI_D_INFO, "OsIndications variable has BOOT_TO_FW_UI bit set\n"));
        RetVal = TRUE;
      }
      else {
        RetVal = FALSE;
        DEBUG ((EFI_D_INFO, "OsIndications variable has BOOT_TO_FW_UI bit cleared\n"));
      }
    }
  }
        
  return RetVal;        
}
/* 
  Update bit EFI_OS_INDICATIONS_BOOT_TO_FW_UI if Applicable else API returns
   Bit is set if "ClearBit" argument is TRUE else cleared
 */
VOID
ClearBootToFWUIField (
  IN BOOLEAN ClearBit
  )
{
  EFI_STATUS Status;
  UINT64     OsIndication = 0;
  UINTN      DataSize;
  UINT32     Attributes; 
  //
  // If OsIndications is invalid, remove it.
  // Invalid case
  //   1. Data size != UINT64  
  //   2. OsIndication attribute inconsistence
  //
  OsIndication = 0;
  Attributes = 0;
  DataSize = sizeof(OsIndication);
  Status = gRT->GetVariable (
                  L"OsIndications",
                  &gEfiGlobalVariableGuid,
                  &Attributes,
                  &DataSize,
                  &OsIndication
                  );

  if (!EFI_ERROR(Status)) {
    if (DataSize == sizeof(OsIndication) &&
        Attributes == (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)){
        
        /* Variable is valid. */
        if(OsIndication & EFI_OS_INDICATIONS_BOOT_TO_FW_UI) {
          DEBUG ((EFI_D_INFO, "OsIndications variable has BOOT_TO_FW_UI bit set\n"));

        if(ClearBit == TRUE)
        {
          /* Clear the bit, so that we dont boot into Setup app on next boot */
          OsIndication &= ~EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
        }
        else
        {
          OsIndication |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;        
        }
        Status = gRT->SetVariable (
                          L"OsIndications",
                          &gEfiGlobalVariableGuid,
                          Attributes,
                          sizeof(OsIndication),
                          &OsIndication
                          );
          ASSERT_EFI_ERROR (Status);      
        }
      }
  }
}


/* Send SD card detection event */
VOID
SendEventDetectSdCard(VOID)
{
  EFI_STATUS Status;
  Status = EfiEventGroupSignal(&gEfiEventDetectSdCardGuid);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "Error: Cannot signal Dected Sd Card Event\n"));
  }
}

VOID
LaunchBDSMenu(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  
  DEBUG ((EFI_D_WARN, "Entering BDS Menu\n"));
  
  //Disable watchdog
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // Signal read to boot event
  EfiSignalEventReadyToBoot ();

  // End Perf marker
  PERF_END (NULL, "BDS", NULL, 0);

  // Load Debug FV image here
#ifndef PRE_SIL
  Status = LoadDebugToolsFv ();
  if (Status != EFI_SUCCESS){
     DEBUG ((EFI_D_WARN, "WARNING: Debug Tools FV failed to load, Status : %r", Status));
  }
#endif

  // Send SD card detection event
  SendEventDetectSdCard();

  BdsConnectAllDrivers ();

  SerialPortDrain ();
  EnableSynchronousSerialPortIO ();

  // Launch Bds Menu from Tools FV
  LaunchMenu(NULL);

  // Drop into EBL Shell
  LaunchAppFromGuidedFv(&gToolsFvGuid, "Ebl", NULL);
}      

//
// BDS Platform Functions
//

#define DEF_APP_STR_LEN 64
#define FILE_IN_FV_PREPEND "fv1:\\"
/**
 * Launch default BDS Boot App.
 * If default BDS Boot App is specified then this function should NOT return,
 * on failure to launch or if the launched app returns, launch the shell app
 * if the device is NOT in retail mode as determined by PRODMODE Build and
 * fuse blown status.
*/
EFI_STATUS
EFIAPI
PlatBdsLaunchDefaultApps (VOID)
{

  EFI_STATUS Status = EFI_SUCCESS;
  CHAR8 DefaultApp[DEF_APP_STR_LEN];
  UINTN Size = DEF_APP_STR_LEN;
  CHAR8 FileinFV[64] = {0};

  if(PlatSecurityEnabled  == TRUE )
  {
    DEBUG ((EFI_D_ERROR, "Error: Platform Security not expected to be Set at this point\n"));
    return EFI_ABORTED;    
  }
  
  Status = GetConfigString ("DefaultChargerApp", DefaultApp, &Size);
  AsciiStrCpy (FileinFV, FILE_IN_FV_PREPEND);
  AsciiStrCat (FileinFV, DefaultApp);

  if (Status == EFI_SUCCESS)
  {
    Status = LoadImageFromFV (FileinFV, NULL );
    if (EFI_ERROR(Status))
      DEBUG((EFI_D_ERROR, "Failed to launch default charger app, status: %r\n", Status));
  }

  Size = DEF_APP_STR_LEN;

  Status = GetConfigString ("DefaultBDSBootApp", DefaultApp, &Size);
  if (Status == EFI_SUCCESS)
  {
    DisplayPOSTTime ();
    LaunchAppFromGuidedFv(&gEfiAblFvNameGuid, DefaultApp, NULL);
    //If we return from above function, considered a failure
    ConfirmShutdownOnFailure();
  }
  else
  {
    DEBUG ((EFI_D_INFO, "[QcomBds] No default boot app specified\n"));
  }

  return Status;
}

/**
 * Lock Variables
*/
STATIC
EFI_STATUS
EFIAPI
PlatBdsLockVariablePolicy (VOID)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  VARIABLE_POLICY_PROTOCOL *vp          = NULL;    /* Variable policy protocol */
  
  Status = gBS->LocateProtocol(
      &gVariablePolicyProtocolGuid,
      NULL,
      (VOID **) &vp);  
      
  if (EFI_ERROR(Status) || (vp == NULL))
  {
    DEBUG ((EFI_D_ERROR, "[PlatBdsLockVariablePolicy] Error locating variable policy protocol\n"));
    return Status;
  }
  
  Status = vp->LockVariablePolicy();
  
  return Status;
}

/**
 * Launch default OEM Setup App.
*/
EFI_STATUS
EFIAPI
PlatBdsProcessOsIndicationsForOemSetupApp (VOID)
{   
  CHAR8       DefaultApp[DEF_APP_STR_LEN];
  UINTN       Size = DEF_APP_STR_LEN;  
  EFI_STATUS  Status = EFI_SUCCESS;
  QCOM_PERF_DATA QPerfDat;

  QPERF_START(&QPerfDat); 
  
  Status = GetConfigString ("OEMSetupApp", DefaultApp, &Size); 
  if (EFI_ERROR(Status))
  {
    /* If OEMSetupApp is not set , ignore further processing.
     * Fallback to BDS menu later if applicable.
     */
    return EFI_SUCCESS;
  }

   // Lock Variables before launching OEM Setup app
   Status = PlatBdsLockVariablePolicy();
   if ((Status != EFI_SUCCESS) && (Status != EFI_ALREADY_STARTED))
     return Status;

  SetOsIndicationsSupported();
  
  if(CheckBootToFWUIField())
  {
    ClearBootToFWUIField(TRUE);
    
    DEBUG((EFI_D_WARN, "Launching OEMSetupApp \n"));
    /*Launch Setup App if set */
    Status = LaunchAppFromGuidedFv(&gMainFvGuid, DefaultApp, NULL);
    if (EFI_ERROR(Status))
    {
      /*Set BootToFWUIField on error to enter BDS menu*/
      ClearBootToFWUIField(FALSE);
      DEBUG((EFI_D_ERROR, "Failed to launch default OEM setup app, status: %r\n", Status));
    }
  }
  QPERF_END(&QPerfDat, " PlatBdsProcessOsIndications Delta");

  return Status;
}


/* TODO: Move this code to other place if found better appropriate */
VOID SSCrashHandler (VOID* Arg)
{
  DEBUG ((EFI_D_WARN, (CHAR8*)Arg));

  /* Do what's required to handle crashes */
  ArmDisableInterrupts();

  //DoWatchDogReset();

  while (1);
}

/* TODO: Move the interrupt num config to target specific file */
#define LPASS_CRASH_INTERRUPT_SOURCE                 194
#define MPSS_CRASH_INTERRUPT_SOURCE                  298
#define NPU_CRASH_INTERRUPT_SOURCE                   398

/* Sensor subsys */
#define SLPI_CRASH_INTERRUPT_SOURCE                  526

/* Compute turing subsy */
#define CDSP_CRASH_INTERRUPT_SOURCE                  610


VOID SetupSubSysCrashHandler (VOID)
{
  if (RegisterThrCtxtIntrHandler (LPASS_CRASH_INTERRUPT_SOURCE, &SSCrashHandler, "\n@@@@ LPASS crash Triggered @@@@\n") == EFI_SUCCESS)
    ConfigureInterrupt (LPASS_CRASH_INTERRUPT_SOURCE, INTR_CONFIG_EDGE_TRIGGER, 0);
    UnmaskInterrupt (LPASS_CRASH_INTERRUPT_SOURCE);

  if (RegisterThrCtxtIntrHandler (MPSS_CRASH_INTERRUPT_SOURCE, &SSCrashHandler, "\n@@@@ MPSS crash Triggered @@@@\n") == EFI_SUCCESS)
    ConfigureInterrupt (MPSS_CRASH_INTERRUPT_SOURCE, INTR_CONFIG_EDGE_TRIGGER, 0);
    UnmaskInterrupt (MPSS_CRASH_INTERRUPT_SOURCE);

  if (RegisterThrCtxtIntrHandler (NPU_CRASH_INTERRUPT_SOURCE, &SSCrashHandler, "\n@@@@ NPU crash Triggered @@@@\n") == EFI_SUCCESS)
    ConfigureInterrupt (NPU_CRASH_INTERRUPT_SOURCE, INTR_CONFIG_EDGE_TRIGGER, 0);
    UnmaskInterrupt (NPU_CRASH_INTERRUPT_SOURCE);

  if (RegisterThrCtxtIntrHandler (SLPI_CRASH_INTERRUPT_SOURCE, &SSCrashHandler, "\n@@@@ SLPI crash Triggered @@@@\n") == EFI_SUCCESS)
    ConfigureInterrupt (SLPI_CRASH_INTERRUPT_SOURCE, INTR_CONFIG_EDGE_TRIGGER, 0);
    UnmaskInterrupt (SLPI_CRASH_INTERRUPT_SOURCE);

  if (RegisterThrCtxtIntrHandler (CDSP_CRASH_INTERRUPT_SOURCE, &SSCrashHandler, "\n@@@@ CDSP crash Triggered @@@@\n") == EFI_SUCCESS)
    ConfigureInterrupt (CDSP_CRASH_INTERRUPT_SOURCE, INTR_CONFIG_EDGE_TRIGGER, 0);
    UnmaskInterrupt (CDSP_CRASH_INTERRUPT_SOURCE);
}

/**
*  Detect hot key push for entering
*     - Mass storage mode
*     - UEFI Shell 2.0/EBL
*
*  Note:
*    Enter BDS Menu.
*    Detect hotkey for development purposes
*    If enabled in PROD image this would be a SECURITY HOLE
*    do not enable in prod/retail
*    Also ensure UEFI security is not eabled before menu launch
*/  
EFI_STATUS
EFIAPI
PlatformBdsDetectHotKey (VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_INPUT_KEY Key;
  UINT32   ReadKeyAttrib = 0;
  UINTN    DataSize, GotoMenu = 0;
  UINT8    VarFlag = 0;
  UINT8    IBlkFlag = 0;
  UefiInfoBlkType* UefiInfoBlockPtr = NULL;
  CHAR8 DefaultApp[DEF_APP_STR_LEN];
  UINTN Size = DEF_APP_STR_LEN;    

  //PrintBootOrder();

  if(PlatBdsState >= PLATFORM_SECURITY_SET  || PlatBdsState < POST_CONSOLE_INIT)
  {
    DEBUG ((EFI_D_ERROR, "Error: Hotkeys to be processed before security setup\n"));
    return EFI_ABORTED;
  }

  DataSize = sizeof(VarFlag);
  Status = gRT->GetVariable (BDS_HOTKEY_STATE_VARNAME,
                             &gQcomTokenSpaceGuid,
                             NULL,
                             &DataSize,
                             &VarFlag);

  /* Disable hotkey menu if BDSHotKeyState = 1 */
  if ((Status == EFI_SUCCESS) && (VarFlag == BDSHotKeyStateDisableDetect))
  {
    DEBUG ((EFI_D_WARN, "HotKey Menu Detection Disabled. Skipping....\r\n"));
    goto SkipHotKey;
  }

  ReadKeyAttrib |= READ_KEY_ATTRIB_RESET_AFTER_READ;
  ReadKeyAttrib |= READ_KEY_ATTRIB_NO_BLOCKING;

  Key.ScanCode = SCAN_NULL;
  Key.UnicodeChar = CHAR_NULL;

  // Check if HotKey is found
  Status = ReadAnyKey (&Key, ReadKeyAttrib);

  if ((Status == EFI_SUCCESS) && ((Key.UnicodeChar != CHAR_NULL) || (Key.ScanCode != SCAN_NULL)))
  {
    DEBUG ((EFI_D_INFO, "HotKey: 0x%x, 0x%x\r\n",
            Key.ScanCode, Key.UnicodeChar));

    if (Key.ScanCode == SCAN_HOME)
    {
      ++GotoMenu;
      DEBUG((EFI_D_WARN, "BDS Menu Hotkey detected\n"));
    }
  }
  else
  {

    // If no hotkey then see if other methods force it
    if (VarFlag == BDSHotKeyStateForceMenu)
    {
      ++GotoMenu;
      DEBUG((EFI_D_WARN, "Hotkey forced by Variable\n"));
    }
    
    if(CheckBootToFWUIField())
    {
       /*If OEMSetupApp was set, this bit would have been already checked earlier
       and we would not reach here. 
       Ensure PlatBdsProcessOsIndications gets invoked earlier than this api.
       */
      ++GotoMenu;
      DEBUG((EFI_D_WARN, "Hotkey forced by OSIndications \n"));       
    }    
SkipHotKey:
    UefiInfoBlockPtr = (UefiInfoBlkType*)GetInfoBlkPtr ();
    if (UefiInfoBlockPtr != NULL)
    {
      IBlkFlag = UefiInfoBlockPtr->BdsHotkey;
      if (IBlkFlag)
      {
        UefiInfoBlockPtr->BdsHotkey = 0;
        ++GotoMenu;
        DEBUG((EFI_D_WARN, "Hotkey initiated by debug script\n"));
      }
    }
  }

  if (GotoMenu)
  {
    EnableSynchronousSerialPortIO ();

    DisplayUefiTotalTime ();

    Status = GetConfigString ("OEMSetupApp", DefaultApp, &Size);    
    if ((Status == EFI_SUCCESS))
    {
      DEBUG ((EFI_D_WARN, "Hotkey detected with OEMSetupApp set, entering OEMSetupApp\n"));
      
      // Lock Variables before launching OEM Setup app
      Status = PlatBdsLockVariablePolicy();
      if ((Status != EFI_SUCCESS) && (Status != EFI_ALREADY_STARTED))
        return Status;

      Status = LaunchAppFromGuidedFv(&gMainFvGuid, DefaultApp, NULL);
      if (EFI_ERROR(Status))
        DEBUG((EFI_D_ERROR, "Failed to launch default OEM setup app, status: %r\n", Status));
    }
    else if(!RETAIL)
    {
      DEBUG ((EFI_D_WARN, "Hotkey detected, entering Menu\n"));        
      LaunchBDSMenu();
    }
  }
  
  PlatBdsState = PROCESS_HOT_KEYS;
  /*If this point is reached all processing went ok */
  Status = EFI_SUCCESS;
  
  DEBUG ((EFI_D_INFO, "Continuing with BDS initialization\n"));
  return Status;
}


VOID ConfirmShutdownOnFailure (VOID)
{
  EFI_INPUT_KEY  Key;
  EFI_STATUS Status;
  Print (L"Press any key to shutdown\n");
  Status = ReadAnyKey (&Key, 0);
  if (Status == EFI_SUCCESS)
  {
    Print (L"Key detected, shutting down\n");
    gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
  }
}


/**
  This is called after the BDS exhuasts all options, either
  through enumeration or attempts to boot
**/
VOID
PlatformBdsBootHalt (VOID)
{
  //Last resort if no other bootable option exists
  if (!RETAIL && EnableShellFlag)
  {
    LoadDebugToolsFv ();
    PlatformBdsLoadShellForNonProdMode (NULL);
  }
  ConfirmShutdownOnFailure();
}

/**
  An empty function to pass error checking of CreateEventEx ().

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.

**/
VOID
EFIAPI
ACPITableLoadEmptyFuntion (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

VOID
InitLcdDebugFlag (VOID)
{
  EFI_STATUS                Status;
  UINTN                     VarSize;
  UINT32                    VarData;

  if (RETAIL)
  {
    LcdDebugFlag = 0;  /* Production mode - no messages */
    EnableShellFlag = 0;  /* Production mode - no shell    */
  }
  else
  {
    /* Read DispDebugInfo variable, and handle if it is not present, default no display */
    VarSize = sizeof(UINT32);
    Status = gRT->GetVariable (L"DispDebugInfo", &gQcomTokenSpaceGuid, NULL, &VarSize, &LcdDebugFlag);

    if (EFI_ERROR(Status))
      LcdDebugFlag = 0;  /* Default no info on LCD */

    VarData = 0;
    Status = GetConfigValue ("EnableShell", &VarData);

    if (EFI_ERROR(Status))
      EnableShellFlag = 0; /* Default no Shell*/
    else
      EnableShellFlag = VarData;
  }
}

#define VERSION_STR_LEN  128

/**
  Retrieve version string and send to the console
**/
STATIC
EFI_STATUS
DisplayVersion (VOID)
{
  EFI_STATUS                Status;
  CHAR8                     VersionStr[VERSION_STR_LEN];
  UINTN                     StrSize = VERSION_STR_LEN;
  BOOLEAN                   DispVer = PcdGetBool (VersionDisplay);

  /* Override compile time setting */
  if (LcdDebugFlag == 1)
    DispVer = TRUE;
  else
    DispVer = FALSE;

  Status = gRT->GetVariable (L"FwVerStr", &gQcomTokenSpaceGuid,
                             NULL, &StrSize, VersionStr);
  if (EFI_ERROR (Status))
  {
    if (DispVer)
      AsciiPrint ("\nUEFI Ver   : Invalid or Unset\n");
    else
      DEBUG ((EFI_D_ERROR, "UEFI Ver   : Invalid or Unset\n"));
  }
  else
  {
    if (DispVer)
      AsciiPrint ("\nUEFI Ver   : %a\n", VersionStr);
    else
      DEBUG ((EFI_D_ERROR, "UEFI Ver   : %a\n", VersionStr));
  }

  return EFI_SUCCESS;
}

STATIC
VOID
DisplayUefiTotalTime (VOID)
{
  UINT64 Time;
  UINT32 *UefiStartTime;
  EFI_HOB_GUID_TYPE *GuidHob;

  GuidHob = GetFirstGuidHob (&gEfiStartTimeHobGuid);
  UefiStartTime = GET_GUID_HOB_DATA(GuidHob);

  Time = GetTimerCountms ();

  DEBUG ((EFI_D_ERROR, "UEFI Total : %d ms\n", Time-(*UefiStartTime)));
}

/**
 Display Boot Time in milli seconds
 **/

STATIC
EFI_STATUS
DisplayBootTime (CHAR8* Key, CHAR8* Event, BOOLEAN OnLCD)
{
  UINT64 Time;

  Time = GetTimerCountms ();

  if(LcdDebugFlag && OnLCD)
  {
    AsciiPrint("%a [%5lld] %a\n", Key, Time, Event);
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "%a [%5lld] %a\n", Key, Time, Event));
  }

  return EFI_SUCCESS;
}

VOID
PrintCoreSpeedInfo (VOID)
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL,
                                (VOID**) &pClockProtocol);
  if (Status == EFI_SUCCESS)
  {
    UINT32 FreqHz = 0;
    UINT32 FreqMHz = 0;
    UINT32 nPerfLevel,nRequiredVoltage ;

    Status = pClockProtocol->GetCpuPerfLevel(pClockProtocol, 0, &nPerfLevel); // Silver, core 0
    if (Status == EFI_SUCCESS)
    {
      Status = pClockProtocol->GetCpuPerfLevelFrequency(pClockProtocol, 0, nPerfLevel, &FreqHz, &nRequiredVoltage); // Silver, core 0
      if (Status == EFI_SUCCESS)
      {
        FreqMHz = FreqHz/1000000ULL;
        DEBUG ((EFI_D_INFO, "Silver cluster Core 0 Freq: %d MHz\r\n", FreqMHz));
      }
      else
      {
        DEBUG ((EFI_D_ERROR, "Error Get Silver cluster Core 0 Freq, fail status %d\r\n", Status));
      }
    }
  }
}

/**
  Retrieve platform info and send to the console
**/
STATIC
EFI_STATUS
DisplayPlatformInfo (VOID)
{
  EFI_STATUS         Status;
  CHAR8              DestBuffer[128];
  CHAR8*             PlatTypePtr;
  CHAR8*             ChipNamePtr;
  CHAR8*             ChipVerPtr;
  CHAR8              AsciiString[50];
  UINTN              Sz;
  BOOLEAN            DispVer = PcdGetBool (VersionDisplay);
  EFI_CHIPINFO_PROTOCOL       *pChipInfoProtocol;
  EFIChipInfoSerialNumType    ChipSerialNum;
  EFI_PLATFORMINFO_PROTOCOL   *pPlatformInfoProtocol;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE PlatformInfo;

  /* Override compile time setting */
  if (LcdDebugFlag == 1)
    DispVer = TRUE;
  else
    DispVer = FALSE;

  Status = GetPlatformStrings (DestBuffer, sizeof(DestBuffer),
                               &PlatTypePtr, &ChipNamePtr, &ChipVerPtr);

  if (Status != EFI_SUCCESS)
    return Status;

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL,
                               (VOID **) &pChipInfoProtocol);
  if (EFI_ERROR(Status))
    return Status;

  if ((Status = pChipInfoProtocol->GetSerialNumber(pChipInfoProtocol, 
                                                   &ChipSerialNum)) != EFI_SUCCESS)
    return Status;

  Status = gBS->LocateProtocol ( &gEfiPlatformInfoProtocolGuid, NULL,
                                (VOID **) &pPlatformInfoProtocol);
  if (EFI_ERROR(Status))
    return Status;

  Status = pPlatformInfoProtocol->GetPlatformInfo(pPlatformInfoProtocol,
                                                  &PlatformInfo);

  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_INFO, "GetPlatformInfo failed\n"));
    return Status;
  }

  /* Display to LCD Screen only if needed during development time */
  if (DispVer)
  {
    AsciiPrint ("Platform           : %a\n", PlatTypePtr);
    AsciiPrint ("Subtype            : %d\n", PlatformInfo.subtype);

    if (boot_from_ufs())
      AsciiPrint ("Boot Device        : UFS\n");
    else if (boot_from_emmc())
      AsciiPrint ("Boot Device        : eMMC\n");
    else if (boot_from_sdcc())
      AsciiPrint ("Boot Device        : SDC\n");
    else if (boot_from_nvme())
      AsciiPrint ("Boot Device        : NVME\n");
    else if (boot_from_spi_nor())
      AsciiPrint ("Boot Device        : SPI\n");
    else
    {
      AsciiPrint ("Unable to determine boot device ...\n");
      ASSERT (FALSE);
      CpuDeadLoop();
    }

    AsciiPrint ("Chip Name          : %a\n", ChipNamePtr);
    AsciiPrint ("Chip Ver           : %a\n", ChipVerPtr);
    AsciiPrint ("Chip Serial Number : 0x%x\n", ChipSerialNum);
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "Platform           : %a\n", PlatTypePtr));
    DEBUG ((EFI_D_ERROR, "Subtype            : %d\n", PlatformInfo.subtype));

    if (boot_from_ufs())
      DEBUG ((EFI_D_ERROR, "Boot Device        : UFS\n"));
    else if (boot_from_emmc())
      DEBUG ((EFI_D_ERROR, "Boot Device        : eMMC\n"));
    else if (boot_from_sdcc())
      DEBUG ((EFI_D_ERROR, "Boot Device        : SDC\n"));
    else if (boot_from_nvme())
      DEBUG ((EFI_D_ERROR, "Boot Device        : NVME\n"));
    else if (boot_from_spi_nor())
      DEBUG ((EFI_D_ERROR, "Boot Device        : SPI\n"));
    else
    {
      DEBUG ((EFI_D_ERROR, "Unable to determine boot device ...\n"));
      ASSERT (FALSE);
      CpuDeadLoop();
    }
    
    DEBUG ((EFI_D_ERROR, "Chip Name          : %a\n", ChipNamePtr));
    DEBUG ((EFI_D_ERROR, "Chip Ver           : %a\n", ChipVerPtr));
    DEBUG ((EFI_D_ERROR, "Chip Serial Number : 0x%x\n", ChipSerialNum));
  }


  Sz = AsciiSPrint (AsciiString, sizeof (AsciiString),
                   "\nPlatform : %a\n", PlatTypePtr);

  /* For nul termination */
  ++Sz;

  gRT->SetVariable (L"PlatformInfo", &gQcomTokenSpaceGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS, Sz, AsciiString);

  PrintCoreSpeedInfo();

  return Status;
}

/**
  Updates platform specific variables for use after BDS

**/

VOID
EFIAPI
UpdatePlatformVars(VOID)
{
  EFI_STATUS Status, BootConfigStatus = EFI_NOT_FOUND;
  EFI_HWIO_PROTOCOL *HWIOProtocol = NULL;
  UINTN      DataSize = 0;
  UINT64     Addr = 0;
  FlashDeviceType     Val  = EFI_MAX_FLASH_TYPE;
  CHAR8      *SdccRegBaseName = NULL;
  MemRegionInfo  MemoryRegion;
  UINT32 PropgateSocInfo =0;

  Status = gBS->LocateProtocol(&gEfiHwioProtocolGuid, NULL, (void**)&HWIOProtocol);
  if ((EFI_SUCCESS != Status) || (HWIOProtocol == NULL))
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to locate HWIO Protocol: 0x%08x\n", Status));

  Status = GetConfigValue64("SharedIMEMBaseAddr", &Addr);
  if (Status == EFI_SUCCESS)
  {
    DataSize = sizeof(Addr);
    Status = gRT->SetVariable (L"Shared_IMEM_Base",
                               &gQcomTokenSpaceGuid,
                               EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               DataSize,
                               &Addr);
  }

  Status  = GetMemRegionInfoByName("Kernel", &MemoryRegion);
  if (Status == EFI_SUCCESS)
  {
    DataSize = sizeof(MemoryRegion.MemBase);
    Status = gRT->SetVariable (L"KernelBaseAddr",
                               &gQcomTokenSpaceGuid,
                               EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               DataSize,
                               &(MemoryRegion.MemBase));
                               
    DataSize = sizeof(MemoryRegion.MemSize);
    Status = gRT->SetVariable (L"KernelSize",
                               &gQcomTokenSpaceGuid,
                               EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               DataSize,
                               &(MemoryRegion.MemSize));                               
                               
  }
  else
  {
    Status = GetConfigValue ("PropagateKernelSocInfo", &PropgateSocInfo);
    if ((Status == EFI_SUCCESS) && (PropgateSocInfo == 1))
    {
       DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Kernel info from UEFI Plat cfg 0x%08x \r\n\n", Status));
    }
  }

  if (!HWIOProtocol)
    return;

  Status = HWIOProtocol->MapRegion(HWIOProtocol, "+SLP_CNTR", (UINT8**) &Addr);
  if (EFI_SUCCESS != Status)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to map SLP_CNTR base address: 0x%08x\n", Status));

  DataSize = sizeof(Addr);
  Status = gRT->SetVariable (L"MPM2_SLP_CNTR_ADDR",
                             &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS,
                             DataSize,
                             &Addr);

  if(boot_from_ufs() == TRUE)
  {
  Status = HWIOProtocol->MapRegion(HWIOProtocol, "+UFS_MEM_UFS", (UINT8**) &Addr);
  }
  else if (boot_from_emmc() == TRUE)
  {
    SdccRegBaseName = (CHAR8 *)PcdGetPtr(SDCCRegBase);
    if (SdccRegBaseName != NULL)
    {
      Status = HWIOProtocol->MapRegion(HWIOProtocol, SdccRegBaseName, (UINT8**) &Addr);
    }
  }							 

  if (EFI_SUCCESS != Status)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to map UFS_MEM_UFS_REGS base address: 0x%08x\n", Status));

  DataSize = sizeof(Addr);
  Status = gRT->SetVariable (L"BootDeviceBaseAddr",
                             &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS,
                             DataSize,
                             &Addr);
  if (EFI_SUCCESS != Status)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Set Variable BootDeviceBaseAddr: 0x%08x\n", Status));                             
     
  BootConfigStatus = GetSharedImemBootOption(&Val);
  if (BootConfigStatus != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Shared Imem boot config Value \n", Status));
  }
  else
  {
    DataSize = sizeof(Val);
    Status = gRT->SetVariable (L"SharedImemBootCfgVal",
                               &gQcomTokenSpaceGuid,
                               EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               DataSize,
                               &Val);
    if (EFI_SUCCESS != Status)
      DEBUG ((EFI_D_ERROR, "ERROR: Failed to Set Variable BootConfigRegVal: 0x%08x\n", Status));
  }
}

/**
  Updates the number of boot cycles and prints boot information stored in NV.

**/
VOID
EFIAPI
UpdateNVVars (VOID)
{
  EFI_STATUS Status;
  UINTN DataSize;
  UINT32 VarData = 0;

  // Print Boot Cycles
  DataSize = sizeof(BootCycleCount);
  Status = gRT->GetVariable (L"MTC",
                             &gQcomTokenSpaceGuid,
                             NULL,
                             &DataSize,
                             &BootCycleCount);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_WARN, "Boot Cycles not initialized, set to 0\n"));
    BootCycleCount = 0;
  }
  DEBUG ((EFI_D_WARN, "Boot Cycle : %d\n", BootCycleCount));


  // Print Run Cycles
  DataSize = sizeof(VarData);
  Status = gRT->GetVariable (L"RunCycles",
                             &gQcomTokenSpaceGuid,
                             NULL,
                             &DataSize,
                             &VarData);

  if (Status == EFI_NOT_FOUND)
    VarData = 0;

  ++VarData;
  DataSize = sizeof(VarData);

  Status = gRT->SetVariable (L"RunCycles",
                             &gQcomTokenSpaceGuid,
                             (EFI_VARIABLE_NON_VOLATILE |
                             EFI_VARIABLE_BOOTSERVICE_ACCESS |
                             EFI_VARIABLE_RUNTIME_ACCESS),
                             DataSize,
                             &VarData);

  DataSize = sizeof(VarData);
  
  Status = gRT->GetVariable (L"RunCycles",
                             &gQcomTokenSpaceGuid,
                             NULL,
                             &DataSize,
                             &VarData);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_WARN, "Run Cycles : not set\n"));
    VarData = 0;
  }
  DEBUG ((EFI_D_WARN, "Run  Cycle : %d\n", VarData));
}

/************************************************************************************
 * WARNING:
 *
 * NOTE: Security Alert..!!
 *
 *   The code until the function SetupPlatformSecurity is called, is running
 *   WITHOUT UEFI security enabled yet. So, in production image any Menu/Shell
 *   or app running, before security enabled, will be a security hole.
 *
 *
 */
/* Setup Platform related security */
VOID SetupPlatformSecurity (VOID)
{
  EFI_STATUS    Status;
  UINTN         VarSize;
  UINT8*        Buffer = NULL;
  UINT8         SetupMode, SecureBoot;
  UINT32        TypeGUIDBufferSize;

  /* Assume by default we are in setupmode and Secure Boot is not enabled */
  SetupMode = 1;
  SecureBoot = 0;

  VarSize = sizeof(SetupMode);
  Status = gRT->GetVariable (L"SetupMode", &gEfiGlobalVariableGuid,
                             NULL, &VarSize, &SetupMode);
  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_WARN, "SetupMode Variable does not exist. Will not enable Secure Boot\n"));
  }

  if (SetupMode == 1)
  {
    /* Disable secure boot if in setup mode */
    SecureBoot = 0;
    VarSize = sizeof(SecureBoot);
    Status = gRT->SetVariable (L"SecureBoot", &gEfiGlobalVariableGuid,
                               EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               VarSize, &SecureBoot);
  }
  else
  {
    /* Enable Secure Boot if not in setup mode */
    SecureBoot = 1;
    VarSize = sizeof(SecureBoot);
    Status = gRT->SetVariable (L"SecureBoot", &gEfiGlobalVariableGuid,
                               EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                               VarSize, &SecureBoot);
  }

  /* Post signal here to enable Image Authentication */
  if (SecureBoot == 1)
    SetSecBootRegSignal ();

  /* Set the variable indicating the signature types supported. If Auth routines support
   * more than SigDB lib supports, then the GUID's from Auth routines also need to be appended
   * to this list */
  TypeGUIDBufferSize = 0;
  if ((GetSupportedSignatureTypes (0, &TypeGUIDBufferSize) == SIG_DB_ENUM_BUFFER_SIZE_INSUFFICIENT) &&
     (TypeGUIDBufferSize > 0))
  {
    Buffer = AllocatePool(TypeGUIDBufferSize);

    if (Buffer)
    {
      if (GetSupportedSignatureTypes (Buffer, &TypeGUIDBufferSize) == SIG_DB_ENUM_SUCCESS)
      {
        VarSize = TypeGUIDBufferSize;
        Status = gRT->SetVariable (L"SignatureSupport", &gEfiGlobalVariableGuid,
                                   EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                   VarSize, Buffer);
      }
      FreePool (Buffer);
    }
  }  
  PlatBdsState = PLATFORM_SECURITY_SET;
}

/**
  Library API which registers for the Exit boot services callback
**/
EFI_STATUS
EFIAPI
RegisterEBSCallback
(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Register to Exit Boot Service Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               PlatformBDSExitBSCallback,
                               NULL,
                               &gEfiEventExitBootServicesGuid,
                               &EfiExitBootServicesEvent);
  ASSERT_EFI_ERROR (Status);
  return Status;
}


VOID
MountRequiredFatPartitions(VOID)
{
  EFI_STATUS Status;
  /* Mount EFIESP for boot option enumeration */
  Status = MountFatPartitionExt(NULL, &gEfiPartTypeSystemPartGuid, NULL);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_WARN, "Failed to mount EFI System partition \n"));
  }  
}

VOID
PerformVariableActions (VOID)
{
  EFI_STATUS Status;
  UINTN      DataSize = 0;
  UINT32     EnableDebugLogDump = 0;
  UINT32     AllowNPVarsInRetail = 0;

  /* Look up Variable Services protocol to be used to flush Variable tables
   * during Exit Bootservices. Cache the protocol handle to use just in case
   * if somebody else installs the protocol to spoof and hijack.
   * Its Ok for this call to fail if the protocol is not installed. */
  Status = gBS->LocateProtocol (&gEfiVariableServicesProtocolGuid,
                                NULL,
                                (VOID**) &VariableServicesProtocol);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "Var Store  : WARNING, not persistent\r\n"));
  }
  else
  {
    UpdateNVVars ();
  }

  /* Check if UEFI NV tables are volatile */
  DataSize = sizeof(VolatileTables);
  Status = gRT->GetVariable (L"VolatileTables",
                             &gQcomTokenSpaceGuid,
                             NULL,
                             &DataSize,
                             &VolatileTables);
  if ((Status == EFI_SUCCESS) && (VolatileTables != 0) && (RETAIL))
  {
    Status = GetConfigValue("EnableUefiSecAppDebugLogDump", &EnableDebugLogDump);
    if((Status == EFI_SUCCESS) && (EnableDebugLogDump))
    {
      SaveTZAppLogs();
    }

    Status = GetConfigValue("AllowNonPersistentVarsInRetail", &AllowNPVarsInRetail);
    if((Status == EFI_SUCCESS) && (AllowNPVarsInRetail))
    {
      /* Capture in RETAIL logs */
      DEBUG ((EFI_D_ERROR, "INFO: UEFI NV tables are enabled as VOLATILE!\r\n"));
    }
    else
    {
      /* In production mode, Shutdown if tables are volatile;
        this indicates an error in reading UEFI NV tables */
      DEBUG ((EFI_D_ERROR, "ERROR: UEFI NV Variables not properly initialized. Shutting down.\r\n"));

      gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

      DEBUG ((EFI_D_ERROR, "ERROR: Shutdown request returned back to PerformVariableActions().\r\n"));

      /* Should not reach here */
      CpuDeadLoop();
    }
  }
  else if (VolatileTables != 0)
  {
    DEBUG ((EFI_D_ERROR, "ERROR: UEFI NV tables are enabled as VOLATILE!\r\n"));

    Status = GetConfigValue("EnableUefiSecAppDebugLogDump", &EnableDebugLogDump);
    if((Status == EFI_SUCCESS) && (EnableDebugLogDump))
    {
      SaveTZAppLogs();
    }

    /* If volatile tables are initialized for NV tables,
       check if manual flag is set to enable UEFI NV tables as volatile */
    DataSize = sizeof(EnableVolatileBootOptions);
    Status = gRT->GetVariable (L"EnableVolatileBootOptions",
                               &gQcomTokenSpaceGuid,
                               NULL,
                               &DataSize,
                               &EnableVolatileBootOptions);
  }

  UpdatePlatformVars ();
}

VOID ConnectControllerGuid (EFI_GUID* CtrlGuid)
{
  EFI_HANDLE   *CtrlHandles;
  UINTN         CtrlHandleCnt, i;
  EFI_STATUS    Status;

  CtrlHandles = NULL;

  Status = gBS->LocateHandleBuffer (ByProtocol,
                                    CtrlGuid,
                                    NULL,
                                    &CtrlHandleCnt,
                                    &CtrlHandles);

  for (i = 0; i < CtrlHandleCnt; ++i)
  {
    Status = gBS->ConnectController (CtrlHandles[i], NULL, NULL, TRUE);
  }

  if (CtrlHandles)
    FreePool (CtrlHandles);
}

/**
  Platform Bds Api's for Pre-Console Config
**/
EFI_STATUS
EFIAPI
PlatformBdsPreConsoleInit (VOID)
{
  StartCyclCounter();

  PlatBdsState = PRE_CONSOLE_INIT_DONE;
  return EFI_SUCCESS;
}

/**
  Connect Consoles and Drivers
**/
EFI_STATUS
EFIAPI
PlatBdsConnectCon (VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  QCOM_PERF_DATA QPerfDat;

  QPERF_START(&QPerfDat); 

  /* Make consoles available to Apps via ST */
  ConnectControllerGuid (&gEfiGraphicsOutputProtocolGuid);

  /* Make consoles available to Apps via ST */
  BdsLibConnectAllConsoles ();

  QPERF_END(&QPerfDat, " ConnectConsoles Delta");  
  return Status;
}

void WaitParallelThreads(void)
{
  VOID* Init_Lock;
  UINTN Wait_Start, Wait_End, Wait_Time, i = 0;
  UINT32 uDedicatedCoreEnabled = 0;
  const char *Lock_Info[][2] = 
  {
     /* DXE Name     Lock name */
	 { "Display",   "DispInit"},
	 { "PIL",       "PIL_Lock"},
	 {  NULL,        NULL},
  };

  DisplayBootTime("WaitParallelThreads InIt", "", FALSE);

  GetConfigValue ("DisplayDedicatedCoreExecution", &uDedicatedCoreEnabled);

  while(Lock_Info[i][0] != NULL)
  {
    if ((uDedicatedCoreEnabled == 1) &&
       !(strcmp(Lock_Info[i][0], "Display")))
    {
      /* Skip waiting for the display thread completion when
       * DisplayDedicatedCoreExecution mode is enabled for DisplayDXE.*/
      i++;
      continue;
    }

    Init_Lock = InitLock(Lock_Info[i][1]);
    Wait_Start = GetTimerCountms ();
        DEBUG ((EFI_D_WARN, "Wait start time for %a thread : %d ms\n", Lock_Info[i][0], Wait_Start));

   /* Spend all the time we can before grabbing lock to make sure we can
    * parallelize (stuff before this call) as much stuff with parallel threads initialization */
    Lock (Init_Lock);
    UnLock (Init_Lock);

    Wait_End = GetTimerCountms ();
    Wait_Time = Wait_End - Wait_Start;
    if (Wait_Time > 2)
      DEBUG ((EFI_D_WARN, "Time spent waiting for Parallel thread %a : %d ms\n", Lock_Info[i][0], Wait_Time));
    i++;
  }
}

/**
  Platform Bds init with init options passed for removable boot detect
**/
VOID
EFIAPI
PlatformBdsPostConsoleInit (VOID)
{
  QCOM_PERF_DATA QPerfDat;

  QPERF_START(&QPerfDat);

  WaitParallelThreads();
  /* Handle all variable storage related things */
  PerformVariableActions();

  /* Sample and display system information (if required on LCD display screen) */
  InitLcdDebugFlag ();
  DisplayVersion ();
  DisplayPlatformInfo ();

  if (!RETAIL)
  {
    /* Mount LogFS partition if enabled in the uefi platform config */
    MountFatPartition(L"logfs");
  }
  DEBUG ((DEBUG_ERROR, "-----------------------------\n"));

  PlatBdsState = POST_CONSOLE_INIT;
  QPERF_END(&QPerfDat, " PlatformBdsPostConsoleInit Delta");  
}


/**
  Setup Platform Security
**/
EFI_STATUS
EFIAPI
PlatformSecurityInit (VOID)
{
  QCOM_PERF_DATA QPerfDat;

  QPERF_START(&QPerfDat);
  
  SetupPlatformSecurity ();
  
  QPERF_END(&QPerfDat, " PlatformSecurityInit Delta");
  return EFI_SUCCESS;
}

/**
  Process Capsules
**/
EFI_STATUS
EFIAPI
PlatBdsProcessFwCapsules (VOID)
{
  EFI_STATUS     Status;

  QCOM_PERF_DATA QPerfDat;
  QPERF_START(&QPerfDat);

  if(PlatBdsState < PLATFORM_SECURITY_SET)
  {
    DEBUG ((EFI_D_ERROR, "Error: Platform Security NOT Setup\n"));
    return EFI_ABORTED;
  }

  /*Process Capsules has a dependency on EFIESP to be mounted earlier*/
  MountRequiredFatPartitions();
  
  Status = ProcessCapsules();
  if ((EFI_END_OF_MEDIA == Status) ||
      (EFI_NOT_STARTED == Status)){
    // Clear the error as there are no capsules to process in current boot
    // as this might be a normal boot where there are not capsules to process
    Status = EFI_SUCCESS;
  } else if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_WARN, "Firmware update failed - Status: %r\n", Status));
  } else {
    DEBUG ((EFI_D_INFO, "Completed successfully: Firmware update\n"));
  }

  QPERF_END(&QPerfDat, "DoCapsuleUpdate Delta");
  
  PlatBdsState = PROCESS_FW_CAPSULES;

  /* Failure status need not cause an assert */
  return EFI_SUCCESS;
}
  
/**
  Setup Firmware Provision
**/
EFI_STATUS
EFIAPI
PlatBdsSocPostProc (VOID)
{
  EFI_STATUS                     Status;
  EFI_QCOM_TOOLSUPPORT_PROTOCOL *ToolSupportProtocol = NULL;
  QCOM_PERF_DATA QPerfDat;
  UINT32 MorPpiEndTime, MorPpiStartTime;

  QPERF_START(&QPerfDat);  

  /* It is necessary to run SetupPlatformSecurity() before RunFwUpdateMain() and RunFwProvisionMain()
   * so that payload would be authenticated. It is also necessary to run RunFwUpdateMain() before
   * ACPI loading so that FwUpdate is more robust.
   */
  if(PlatBdsState < PLATFORM_SECURITY_SET)
  {
    DEBUG ((EFI_D_ERROR, "Error: Platform Security NOT Setup\n"));
    return EFI_ABORTED;
  }

  // Signal ACPI Table Loading
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               ACPITableLoadEmptyFuntion,
                               NULL,
                               &gEfiACPITableLoadGuid,
                               &ACPITableLoadEvent);
  if (Status == EFI_SUCCESS)
    gBS->SignalEvent (ACPITableLoadEvent);

  /* ACPI tables should be loaded at this point. It is necessary to run
   * RunFwProvisionMain() after ACPI loading so that provisioning routine
   * can access RSDP pointer in EFI configuration table and save it for
   * HLOS app to consume.
   */
  DEBUG ((EFI_D_INFO, "Attempting to start: Firmware provisioning\n"));
  Status = RunFwProvisionMain (gImageHandle, gST);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_WARN, "Firmware provisioning failed\n"));
  }

  // Locate EFI_TOOL_SUPPORT_PROTOCOL.
  Status = gBS->LocateProtocol (&gQcomToolSupportProtocolGuid,
                                NULL,
                                (VOID**)&ToolSupportProtocol);

  // Security calls
  if ((Status == EFI_SUCCESS) && (ToolSupportProtocol != NULL ))
  {
    MorPpiStartTime = GetTimerCountms();
    ToolSupportProtocol->HandleMorPpi (ToolSupportProtocol);
    MorPpiEndTime = GetTimerCountms();
    DEBUG((EFI_D_ERROR, " HandleMorPpi delta: %d\n", MorPpiEndTime - MorPpiStartTime));
    
    if ((ToolSupportProtocol->TzFuseMilestone (ToolSupportProtocol) != EFI_SUCCESS) ||
        (ToolSupportProtocol->NeedQcomPkProtection (ToolSupportProtocol) == TRUE))
    {
      gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
      /* Should not reach here */
      CpuDeadLoop();
    }
  }

  /* Fix System Table CRC after updating Firmware Revision */
  gST->Hdr.CRC32 = 0;
  gBS->CalculateCrc32 ((VOID *)gST, sizeof(EFI_SYSTEM_TABLE), &gST->Hdr.CRC32);

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid,
                                NULL,
                                (VOID**)&pSCMProtocol);
  if (EFI_ERROR(Status))
  {
     DEBUG ((EFI_D_ERROR, "Register SCM protocol failed, Status =  (0x%x)\r\n", Status));
  }

  DEBUG ((DEBUG_ERROR, "Platform Init End : %d\n-----------------------------\n",
                      GetTimerCountms ()));

  /* Dump Log Buffer to File */
  Status = WriteLogBufToPartition ();
  if (EFI_ERROR(Status))
  {
     DEBUG ((EFI_D_WARN, "Write Log Buffer to FAT partition failed, Status = (%r)\r\n", Status));
  }

  /* Drain serial port finally */
  SerialPortDrain ();

  /* Validate and take any action for the HW platform configuration */
  ValidateHWConfig ();  

  PlatBdsState = BDS_SOC_POST_PROC;
  
  QPERF_END(&QPerfDat, " PlatBdsSocPostProc Delta");  
  /* Failure status need not cause an assert */
  return EFI_SUCCESS;
}

/**
  API to Handle security post firmware provisioning
**/
EFI_STATUS
EFIAPI
PlatBdsPostFwConfigSec (VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  QCOM_PERF_DATA QPerfDat;

  QPERF_START(&QPerfDat); 
  
  /* ACPI tables should be loaded before SetMeasureBootStartSignal()
   * and SetMeasureBootRegSignal().
   * Start measuring and register
   */
  SetMeasureBootStartSignal ();

  /* Register measure boot values */
  SetMeasureBootRegSignal ();

  PlatBdsState = POST_FW_CONFIG_SEC;
  
  QPERF_END(&QPerfDat, " PlatBdsPostFwConfigSec Delta");  
  return Status;
}


/**
  Hook point to boot from eMMC, if booting from BootOrder
  failed. We don't expect a boot option to return, so the UEFI
  2.0 specification defines that you will default to an
  interactive mode and stop processing the BootOrder list in
  this case. This is also a platform implementation and can be
  customized by IBV/OEM.

  @param  Option                  Pointer to Boot Option that succeeded to boot.

**/
VOID
EFIAPI
PlatformBdsLoadShellForNonProdMode (IN  BDS_COMMON_OPTION *Option)
{
  BOOLEAN ProdModeEnabled = (RETAIL);

  if (ProdModeEnabled)
  {
    ASSERT (FALSE);
    CpuDeadLoop ();
  }

  /* End Perf marker */
  PERF_END (NULL, "BDS", NULL, 0);

  SerialPortDrain ();
  EnableSynchronousSerialPortIO ();

  /* TODO: Conditionalize these to loading only from specific GUIDed FVs
   * that is are already authenticated
   * */
  // Drop to UEFI Shell
  DisplayPOSTTime ();

  //PrintBootOrder();

  DEBUG ((EFI_D_WARN, "Attempting to start: Ebl\n"));
  LaunchAppFromGuidedFv(&gToolsFvGuid, "Ebl", NULL);

  DEBUG ((EFI_D_WARN, "Attempting to start: Shell\n"));
  LaunchAppFromGuidedFv(&gToolsFvGuid, "Shell", NULL);
}

/**
  Function that executes just before loading a boot option image.

  @param  Option                  Pointer to Boot Option that succeeded to boot.

**/
EFI_STATUS
EFIAPI
PlatformBdsPreLoadBootOption (IN  BDS_COMMON_OPTION *Option)
{
  BOOLEAN ProdModeEnabled = (RETAIL);

  /* Do not allow boot option to boot if in production scenario */
  if ((VolatileTables != 0) && (ProdModeEnabled || (!EnableVolatileBootOptions)))
    return EFI_DEVICE_ERROR;

  if (Option == NULL)
    return EFI_INVALID_PARAMETER;

  if (Option->Description == NULL)
    DEBUG ((DEBUG_INFO | DEBUG_LOAD | DEBUG_ERROR, "Booting option %d:(Boot%04x) from unknown device path\n", Option->OptionNumber, Option->BootCurrent));
  else
    DEBUG ((DEBUG_INFO | DEBUG_LOAD | DEBUG_ERROR, "Booting option %d:(Boot%04x) \"%S\"\n", Option->OptionNumber, Option->BootCurrent, Option->Description));

  DisplayPOSTTime();

  return EFI_SUCCESS;
}

/**
  This Function writes UEFI Log Buffer to LOGFS Partition

  @retval EFI_SUCCESS     File was written successfully
**/
#define MAX_LEN 13
#define FILE_EXT L".txt"
EFI_STATUS
EFIAPI
WriteLogBufToPartition (VOID)
{
  EFI_STATUS Status;
  UINT32 VarData;
  CHAR16 LogBufFile[MAX_LEN] = L"UefiLog";
  UefiInfoBlkType* UefiInfoBlockPtr = NULL;
  UINTN Length;
  
  BOOLEAN                   bOffModeCharging   = FALSE;
  EFI_CHARGER_EX_PROTOCOL   *ChargerExProtocol = NULL;  
  
  if(boot_from_nvme() && (EFI_SUCCESS == gBS->LocateProtocol(&gChargerExProtocolGuid, NULL, (void**)&ChargerExProtocol)))
  {
      if((EFI_SUCCESS == ChargerExProtocol->IsOffModeCharging(&bOffModeCharging)) && bOffModeCharging)
      {
        DEBUG ((EFI_D_ERROR, "OffModeCharging and NVME device : Skipping logs flush to LOGFS partition \n"));
        return EFI_UNSUPPORTED;
      }
  }      

  UefiInfoBlockPtr = (UefiInfoBlkType*)GetInfoBlkPtr ();
  if (UefiInfoBlockPtr == NULL)
  {
    DEBUG ((EFI_D_ERROR, "GetInfoBlkPtr() invalid!"));
    return EFI_NOT_FOUND;
  }

  /* Only keep Logs of past 5 Boot Cycles
     File number corresponds to current BootCycle (0-4) */
  VarData = BootCycleCount % 5;
  VarData = VarData + '0';  /* Convert int to char */
  /* Construct File name */
  StrCat (LogBufFile, (CHAR16*)&VarData);
  StrCat (LogBufFile, FILE_EXT);

  Length = UefiInfoBlockPtr->UartLogBufferLen;
  if (Length == 0 || UefiInfoBlockPtr->UartLogBufferPtr == NULL)
  {
    DEBUG ((EFI_D_ERROR, "RAM Log Buffer is empty!"));
    return EFI_NOT_FOUND;
  }

  /* Write Logs to Partition */
  Status = WriteFile (LogBufFile,
                      NULL,
                      NULL,
                      L"logfs",
                      TRUE,
                      NULL,
                      &Length,
                      0,
                      (UINT8*)UefiInfoBlockPtr->UartLogBufferPtr,
                      UefiInfoBlockPtr->UartLogBufferLen);
  if (Length != UefiInfoBlockPtr->UartLogBufferLen)
  {
    DEBUG ((EFI_D_ERROR, "Unable to write complete log to partition. Request:0x%08x Written:0x%08x\n",
      UefiInfoBlockPtr->UartLogBufferLen, Length));
  }
  return Status;
}


VOID
EFIAPI
InternalEmptyCallback( 
    IN EFI_EVENT  Event,
    IN VOID      *Context
  )
{
}

EFI_STATUS
ExecImageFromNetwork(VOID)
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Image;
  EFI_DEVICE_PATH_PROTOCOL       *DevicePath;
  UINTN                          ExitDataSize;
  CHAR16                         *ExitData;
  UINTN                          NumOfLoadFileHandles = 0;
  EFI_HANDLE                     *LoadFileHandles;
  UINTN                          Index;
  
  EFI_EVENT                      EthernetDriverEvent;
  Status = gBS->CreateEventEx (
                   EVT_NOTIFY_SIGNAL,
                   TPL_CALLBACK,
                   InternalEmptyCallback,
                   NULL,
                   &gEfiEthernetEventGuid,
                   &EthernetDriverEvent
                   ); 
  Status = gBS->SignalEvent(EthernetDriverEvent);
  
  BdsConnectAllDrivers ();
  
  // Search Load File protocol for PXE boot option.
  Status = gBS->LocateHandleBuffer(ByProtocol,
                          &gEfiLoadFileProtocolGuid,
                          NULL,
                          &NumOfLoadFileHandles,
                          &LoadFileHandles);
                          
  if ((!EFI_ERROR(Status)) && (LoadFileHandles != NULL))
  {
    for (Index = 0; Index < NumOfLoadFileHandles; Index++)
    {
      AsciiPrint("\nStart PXE Boot ......\n");

      // Get the device path from the handle
      DevicePath = DevicePathFromHandle(LoadFileHandles[Index]);
      
      // Send Ready to boot event
      EfiSignalEventReadyToBoot();

      // Load the image into memory
      Status = gBS->LoadImage(TRUE, gImageHandle, DevicePath, NULL, 0, &Image);

      if (!EFI_ERROR(Status))
      {
        // Transfer control to the image we loaded with LoadImage()
        Status = gBS->StartImage(Image, &ExitDataSize, &ExitData);
      }
    }

    // Free the buffer allocated from the pool
    FreePool (LoadFileHandles);
  }
  
  return Status;
}
