/** @file EnvDxe.c

  UEFI Environment Dxe code for registering FBPT event handlers
  For all Qualcomm specific initializations

  Copyright (c) 2012-2019 Qualcomm Technologies, Inc.
  All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 11/01/19    am       Added notification event for populating RuntimeDbgTablePtr
 08/30/19    am       Added GetInstalledPhysicalMemory function
 05/31/19    nm       Update Call to Set UART Baud Rate
 03/12/19    kpa      Set DDR Freq vote to 1.5 Ghz
 10/04/18    kpa      Rename Power Protocol to QcomEnv Protocol
 10/03/18    kpa      Added Power Protocol
 09/13/18    md       "Exit BS" timetamp logging moved at the end of ExitBootServices().
 07/08/17    mt       added TZ_SET_MILESTONE scm call, in ExitBootServices callback
 02/08/18    bd       Add option to enable HS UART
 10/10/17    c_vkuppa Register 32bit mode switch protocol
 05/05/17    sj	      removing the FBPT lib dependency
 12/05/16    vk       Skip registering FBPT if disabled in dsc
 10/31/16    bh       Register callback to render splash screen
 10/21/16    ai       Add Watchdog support
 09/09/15    vk       Add MMU lib
 03/03/16    bh       Enable print shared lib
 01/07/16    bh       Add support for GPT partition update
 10/28/15    ai       DEP Support
 10/14/15    vk       Add NULL pointer checks
 09/30/15    bh       Update system table with FW version hex
 10/12/15    vk       Add RAM partition table lib 
 09/30/15    vk       Add RAM partition table protocol
 09/17/15    vk       Disable log message
 09/14/15    bh       Add RegisterSerialLogFlush
 09/04/15    vk       Merge flush log changes
 07/14/15    dg       Convert memory to runtime
 07/13/15    al       Changing TPL_NOTIFY to TPL_CALLBACK
 10/27/14    jb       Change FBPT base to uint64
 05/08/14    vk       Add 64 bit support
 04/04/2014  vk       Warning cleanup
 15/10/2013  vk       Disable FBPT
 10/04/2013  yg       Add Dxe Info into InfoBlk to load symbols
 03/04/2013  yg       Make runtime and improve robustness
 29/03/2013  vk       Add support for run time symbol load
 15/03/2013  vk       Move to EnvDxe, add gST to InfoBlock
 07/06/2012  vishalo  Initial revision
=============================================================================*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/DebugImageInfoTable.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Protocol/Cpu.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>

#include <Include/UefiInfoBlk.h>
#include <Library/CookieLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Protocol/EFIRamPartition.h>
#include <Library/ProcAsmLib.h>
#include <Library/DbiDumpLib.h>
#include <Library/WatchdogLib.h>
#include <Library/QcomTargetLib.h>
#include <Protocol/WatchdogTimer.h>
#include <Include/scm_sip_interface.h>
#include <Library/QcomLib.h>
#include <Library/TzRuntimeLib.h>
#include <Include/Protocol/EFIScm.h>
#include <Protocol/EFIDisplayUtils.h>
#include <Protocol/EFINpa.h>
#include <api/systemdrivers/icbarb.h>
#include <Protocol/EFIQcomEnv.h>

#include <Protocol/ReportStatusCodeHandler.h>
#include <Include/Lpm.h>
#include <Protocol/EFIDDRGetConfig.h>

#define DEP_DEBUG_LEVEL EFI_D_INFO
#define ROUND_UP_1MB(VALUE) ( ( (VALUE) + 0xFFFFF ) & ~0xFFFFF )

#ifdef MDEPKG_NASSERT
  #error "Disabling ASSERT disables necessary error checking and is a security risk"
#endif 

#define DRAIN_TIMER_MS                10                        /* Serial drain timer in mS */


/* Event to signal */
EFI_EVENT  DrainSerialTimerEvent;
EFI_EVENT  ExitBootServicesEvent;
EFI_EVENT  BlockIoRefreshEvent;
EFI_EVENT  SplashScreenEvent;

VOID      *CpuProtocolNotificationToken = NULL;
EFI_EVENT CpuProtocolNotificationEvent  = (EFI_EVENT)NULL;
VOID      *TimerArchProtocolNotificationToken = NULL;
EFI_EVENT TimerArchProtocolNotificationEvent = (EFI_EVENT) NULL;
VOID      *GraphicsProtocolNotificationToken = NULL;
EFI_EVENT GraphicsProtocolNotificationEvent = (EFI_EVENT) NULL;

/* Event Guids */
extern EFI_GUID gEfiEventVirtualAddressChangeGuid;
extern EFI_GUID gEfiEventExitBootServicesGuid;
extern EFI_GUID gEfiTimerArchProtocolGuid;
extern EFI_GUID gQcomFwVersionHexGuid;
extern EFI_GUID gBlockIoRefreshGuid;


/* NOTE: This structure size, member POS has dependency
 * in T32 Debug script */
typedef struct DbgTable
{
  EFI_PHYSICAL_ADDRESS PhyLoadAddr;
  EFI_VIRTUAL_ADDRESS VirLoadAddr;

  /* Only the past Build folder portion is stored,
   * 240 will make sure our structure will be of size
   * 256 aligned */
  CHAR8 DriverName[496];
} DriverList;

extern EFI_GUID gEfiDebugImageInfoTableGuid;

STATIC UefiInfoBlkType *InfoBlkPtr;

// Initialized to a value close to the number of runtime drivers we expect to load,
// Buffers will resize if this number is exceeded
#define INIT_DRIVER_COUNT   10
STATIC UINTN CurrRtDbgDriverCnt = INIT_DRIVER_COUNT;
STATIC  DriverList *RtDbgDrivers = NULL;
STATIC UINT8       HsUartBaudRateVar = 0; 


// Global Npa Handle shared with QcomEnv Protocol
// Needs to be shared because QcomEnv protocol needs to use
// same handle to devote DDR CPU busbandwidth request
npa_client_handle reqClientDDR = NULL;


VOID InitDbgDriverList (VOID);
extern VOID SaveCpuCtxt(VOID);

EFI_STATUS GetImageNames (EFI_DEBUG_IMAGE_INFO  *DebugTable, UINTN NumEntries);

VOID EFIAPI ReadyToBootCallBack (EFI_EVENT  Event,  VOID  *Context)
{
  EFI_STATUS  Status;
  UINTN       UefiVarSize; 

  UefiVarSize = sizeof(HsUartBaudRateVar); 
  Status = gRT->GetVariable(L"SetHsUartBaudRate", &gQcomTokenSpaceGuid, NULL, &UefiVarSize, &HsUartBaudRateVar);
  if (EFI_ERROR(Status)) { 
    DEBUG((EFI_D_INFO, "Reading of SetHsUartBaudRate failed ")); 
    HsUartBaudRateVar = 0;
  }  
}

VOID DisablePeriodicSerialDrain(VOID)
{
  gBS->CloseEvent(DrainSerialTimerEvent);
}

// NUM_PARAMS: Size of the Parameters Array
// Note: The parameters are just spare and no values need to be populated, they are present for backward compatibility.
#define NUM_PARAMS 2
VOID EnvExitBootServicesCallback(IN EFI_EVENT Event, IN VOID *Context)
{
  ClearUefiCrashCookie();
  UINT32  Before = 0, After = 0;  
  UINT64 Parameters[SCM_MAX_NUM_PARAMETERS] = { 0 };
  EFI_STATUS  Status = EFI_SUCCESS;
 
  UINT64  BaudRate = UART_BAUD_RATE_3MBPS;
    
  DisablePeriodicSerialDrain();

  Status = TzFastcall(
    TZ_MILESTONE_SET_ID,
    TZ_MILESTONE_SET_ID_PARAM_ID,
    Parameters,
    NUM_PARAMS
  );
  if(Status != EFI_SUCCESS)
  {
	 DEBUG((EFI_D_ERROR, " TZ Milestone call failed, Status =  (0x%x)\r\n", Status));
  }
  
  //Change UART baud rate to HS
  if (HsUartBaudRateVar != 0) { 
    DEBUG((EFI_D_WARN, "enabling UART @ 3Mbps baud rate")); 
    SerialPortFlush(); 
    gBS->Stall(5000);   //stall for 5ms to flush out 
    SerialPortSetBaudRate(&BaudRate); 
  }  

  Before = GetTimerCountms();
  SerialPortFlush();
  After = GetTimerCountms();
  DEBUG ((EFI_D_ERROR, "App Log Flush : %d ms\n", After-Before));
  SerialPortFlush();
}

VOID
SerialPortDrainCallback(IN EFI_EVENT Event, IN VOID *Context)
{
  SerialPortDrain();
}

VOID 
EnablePeriodicSerialDrain(  
  IN EFI_EVENT   Event,
  IN VOID        *Context
)
{
  EFI_STATUS Status;
  Status = gBS->CreateEvent(
      EVT_TIMER | EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      SerialPortDrainCallback,
      NULL,
      &DrainSerialTimerEvent
      );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "Serial flush event creation failed\n"));
    return;
  }

  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL, 
      TPL_CALLBACK,
      EnvExitBootServicesCallback,
      NULL,
      &gEfiEventExitBootServicesGuid,
      &ExitBootServicesEvent
      ); 
  
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "ExitBootServices regtistration failed\n"));
    return;
  }

  Status = gBS->SetTimer(DrainSerialTimerEvent, TimerPeriodic, EFI_TIMER_PERIOD_MILLISECONDS(DRAIN_TIMER_MS)); 
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "Serial flush timer: Failed\n"));
  }
  return;
}

EFI_STATUS
RegisterSerialLogFlush (VOID)
{
  EFI_STATUS Status;

  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, EnablePeriodicSerialDrain, NULL, &TimerArchProtocolNotificationEvent);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (&gEfiTimerArchProtocolGuid, TimerArchProtocolNotificationEvent, (VOID *)&TimerArchProtocolNotificationToken);
  ASSERT_EFI_ERROR (Status);
  return Status;
}


EFI_STATUS UpdateInfoBlkSystemTableAddr (VOID)
{
  if (InfoBlkPtr)
    InfoBlkPtr->SystemTablePtr = (UINTN*) gST;

  return EFI_SUCCESS;
}

VOID
InitDbgDriverList (VOID)
{
  EFI_CONFIGURATION_TABLE *EfiCfgTblPtr = NULL;
  EFI_DEBUG_IMAGE_INFO_TABLE_HEADER *EfiDbgImgInfTbl = NULL;
  UINTN NumDbgImages = 0;
  UINTN NumConfigTblEntries = 0;

  UINTN i;

  NumConfigTblEntries = gST->NumberOfTableEntries;
  EfiCfgTblPtr = gST->ConfigurationTable;

  /* Look at all configuration table entries for debug table */
  for (i = 0; i < NumConfigTblEntries; i++)
  {
    if(CompareGuid (&EfiCfgTblPtr->VendorGuid, &gEfiDebugImageInfoTableGuid) == TRUE)
    {
      EfiDbgImgInfTbl = (EFI_DEBUG_IMAGE_INFO_TABLE_HEADER*) EfiCfgTblPtr->VendorTable;
      NumDbgImages = EfiDbgImgInfTbl->TableSize;

      if (InfoBlkPtr)
      {
        InfoBlkPtr->DebugTablePtr = (UINTN*) EfiDbgImgInfTbl->EfiDebugImageInfoTable;
        InfoBlkPtr->DebugTableEntryCnt = NumDbgImages;
      }
      break;
    }
    else
      EfiCfgTblPtr++;
  }
}


EFI_STATUS
GetImageNames (EFI_DEBUG_IMAGE_INFO  *DebugTable, UINTN NumEntries)
{
  volatile UINTN Count = 0;
  volatile UINTN RtDbgDrvCnt = 0;
  UINTN Entry;

  if ((DebugTable == NULL) || (RtDbgDrivers == NULL))
    return EFI_INVALID_PARAMETER;

  for (Entry = 0; Entry < NumEntries; Entry++, DebugTable++)
  {
    if (DebugTable->NormalImage != NULL)
    {
      if ((DebugTable->NormalImage->ImageInfoType == EFI_DEBUG_IMAGE_INFO_TYPE_NORMAL) &&
          (DebugTable->NormalImage->LoadedImageProtocolInstance != NULL))
        {
          STATIC CHAR8* pName;
          pName = PeCoffLoaderGetPdbPointer (DebugTable->NormalImage->LoadedImageProtocolInstance->ImageBase);

          if (DebugTable->NormalImage->LoadedImageProtocolInstance->ImageCodeType == EfiRuntimeServicesCode)
          {
            if (RtDbgDrvCnt >= CurrRtDbgDriverCnt) {
              AllocateCopyPool(((CurrRtDbgDriverCnt*2) * sizeof(DriverList)), (VOID**)&RtDbgDrivers);
              CurrRtDbgDriverCnt*=2;
            }
  
            if (pName != NULL)
              AsciiStrnCpy (RtDbgDrivers[RtDbgDrvCnt].DriverName, pName, sizeof(RtDbgDrivers[RtDbgDrvCnt].DriverName));
  
            RtDbgDrivers[RtDbgDrvCnt].PhyLoadAddr = (EFI_PHYSICAL_ADDRESS) DebugTable->NormalImage->LoadedImageProtocolInstance->ImageBase;
            RtDbgDrivers[RtDbgDrvCnt].VirLoadAddr = (EFI_PHYSICAL_ADDRESS) DebugTable->NormalImage->LoadedImageProtocolInstance->ImageBase;
            RtDbgDrvCnt++;
            continue;
          }
          Count++;
       }
    }
  }

  if (InfoBlkPtr)
    InfoBlkPtr->RtDbgTableEntryCnt = RtDbgDrvCnt;

  return EFI_SUCCESS;
}

EFI_EVENT VirtualAddressChangeEvent = NULL;

VOID
EFIAPI
VirtualAddressChangeCallBack (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN i;
  EFI_STATUS Status;
  volatile UINTN Errors = 0;

  for (i = 0; i < CurrRtDbgDriverCnt; i++)
  {
    Status = gRT->ConvertPointer(0, (VOID**)&RtDbgDrivers[i].VirLoadAddr);
    RtDbgDrivers[i].VirLoadAddr += 0xDA0;
    if (Status != EFI_SUCCESS)
      Errors++;
  }
}

EFI_EVENT EBSGetImageNameEvent = NULL;

VOID
EFIAPI
EBSGetImageName (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  InitDbgDriverList ();

  if (InfoBlkPtr)
    GetImageNames ((EFI_DEBUG_IMAGE_INFO*)InfoBlkPtr->DebugTablePtr, InfoBlkPtr->DebugTableEntryCnt);
  
  return;
}

EFI_STATUS
InitRuntimeDbg (VOID)
{
  EFI_STATUS Status;

  //Register notification event
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EBSGetImageName,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &EBSGetImageNameEvent
                  );
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_WARN, "EnvDxe: ERROR, Unable to register GetImageName callback, runtime_symbol_load will not be functional\n"));
    return Status;
  }
  
  Status = gBS->AllocatePages (AllocateAnyPages,
                                EfiRuntimeServicesData,
                                EFI_SIZE_TO_PAGES((CurrRtDbgDriverCnt) * sizeof(DriverList)),
                                (VOID*)&RtDbgDrivers);
  CurrRtDbgDriverCnt = EFI_PAGE_SIZE / sizeof(DriverList);

  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_WARN, "EnvDxe: ERROR, unable to allocate memory for runtime debug table\n"));
    return Status;
  }

  SetMem (RtDbgDrivers, ((CurrRtDbgDriverCnt) * sizeof(DriverList)), 0);

  if (InfoBlkPtr)
    InfoBlkPtr->RuntimeDbgTablePtr = (UINTN*)RtDbgDrivers;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  VirtualAddressChangeCallBack,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &VirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
InitMmioRuntime (VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR Descriptor;
  EFI_HOB_MEMORY_ALLOCATION *MemAllocHob;
  UINTN Address;
  UINTN Size;

  MemAllocHob = GetFirstHob (EFI_HOB_TYPE_MEMORY_ALLOCATION);

  while (MemAllocHob)
  {
    if (MemAllocHob->AllocDescriptor.MemoryType == EfiRuntimeServicesData)
    {
      Address = MemAllocHob->AllocDescriptor.MemoryBaseAddress;
      Size = MemAllocHob->AllocDescriptor.MemoryLength;
      Status = gDS->GetMemorySpaceDescriptor(Address, &Descriptor);
      if (EFI_ERROR(Status)) {
        continue;
      }
      if (Descriptor.GcdMemoryType == EfiGcdMemoryTypeMemoryMappedIo) {
        Status = gDS->SetMemorySpaceAttributes(Address, Size, Descriptor.Attributes | EFI_MEMORY_RUNTIME);
      }
    }
    MemAllocHob = (EFI_HOB_MEMORY_ALLOCATION *)((UINTN)MemAllocHob + MemAllocHob->Header.HobLength);
    MemAllocHob = GetNextHob (EFI_HOB_TYPE_MEMORY_ALLOCATION, MemAllocHob);
  }


  return Status;
}

VOID
AddDxeSymbolInfo (VOID)
{
  EFI_HOB_MEMORY_ALLOCATION *MemAllocHob;

  MemAllocHob = GetFirstHob (EFI_HOB_TYPE_MEMORY_ALLOCATION);

  while (MemAllocHob)
  {
    if (MemAllocHob->AllocDescriptor.MemoryType == EfiBootServicesCode)
    {
      InfoBlkPtr->DxeSymbolAddr = (UINT64) MemAllocHob->AllocDescriptor.MemoryBaseAddress;
      break;
    }
    MemAllocHob = (EFI_HOB_MEMORY_ALLOCATION *)((UINTN)MemAllocHob + MemAllocHob->Header.HobLength);
    MemAllocHob = GetNextHob (EFI_HOB_TYPE_MEMORY_ALLOCATION, MemAllocHob);
  }
}

STATIC
VOID
Dep32Handler (VOID)
{
  STATIC EFI_MEMORY_DESCRIPTOR   *MemoryMap = NULL;
  EFI_STATUS              Status;
  EFI_CPU_ARCH_PROTOCOL   *Cpu;
  UINTN                   MemoryMapSize = 0;
  UINTN                   MapKey = 0;
  UINTN                   DescriptorSize = 0;
  UINT32                  DescriptorVersion = 0;
  UINTN                   Pages;
  UINTN                   Index;

  UINT64                EntrySize;
  EFI_PHYSICAL_ADDRESS   StartAddr = 0;
  EFI_PHYSICAL_ADDRESS   RemainingSize = 0;
  
  //
  // Get the cpu protocol that this driver requires.
  //
  Status = gBS->LocateProtocol(&gEfiCpuArchProtocolGuid, NULL, (VOID **)&Cpu);
  ASSERT_EFI_ERROR(Status);

  Status = gBS->GetMemoryMap (
                  &MemoryMapSize,
                  MemoryMap,
                  &MapKey,
                  &DescriptorSize,
                  &DescriptorVersion
                  );
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    Pages = EFI_SIZE_TO_PAGES (MemoryMapSize) + 1;
    MemoryMap = AllocatePages (Pages);
    if ( MemoryMap == NULL )
      return;
  
    Status = gBS->GetMemoryMap (
                    &MemoryMapSize,
                    MemoryMap,
                    &MapKey,
                    &DescriptorSize,
                    &DescriptorVersion
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG(( EFI_D_WARN, "Error: Unable to get memory map\n"));
      return;
    }
  }
  else
  {
    ASSERT (Status != EFI_BUFFER_TOO_SMALL);
    DEBUG(( EFI_D_WARN, "Error: Unable to get memory map size\n"));
    return;
  }


  DEBUG ((DEP_DEBUG_LEVEL, "\n----------------------------------------------------------------------------"));
  DEBUG ((DEP_DEBUG_LEVEL, "\n                                    Map"));
  DEBUG ((DEP_DEBUG_LEVEL, "\n----------------------------------------------------------------------------"));
  for (Index = 0; Index < MemoryMapSize/DescriptorSize; Index++) 
  {
    StartAddr = 0;
    RemainingSize = 0;
    
    EntrySize = LShiftU64 (MemoryMap->NumberOfPages, 12);
    DEBUG ((DEP_DEBUG_LEVEL, "\n %01x %016lx - %016lx: # %08lx %016lx ", MemoryMap->Type, MemoryMap->PhysicalStart, MemoryMap->PhysicalStart+EntrySize, MemoryMap->NumberOfPages, MemoryMap->Attribute));

    if ( (MemoryMap->Type == EfiBootServicesCode)    || 
         (MemoryMap->Type == EfiRuntimeServicesCode) ||
         (MemoryMap->Type == EfiLoaderCode) )
    {
      DEBUG ((DEP_DEBUG_LEVEL, "\t [NXN]"));
      MemoryMap = NEXT_MEMORY_DESCRIPTOR (MemoryMap, DescriptorSize);
      continue;
    }
	
    DEBUG ((DEP_DEBUG_LEVEL, "\t [XN]\n"));

    /* If less than 1 MB */
    if (EntrySize <= SIZE_1MB)
    {
      Status = Cpu->SetMemoryAttributes (Cpu, MemoryMap->PhysicalStart, EntrySize, EFI_MEMORY_XP);
      MemoryMap = NEXT_MEMORY_DESCRIPTOR (MemoryMap, DescriptorSize);
      continue;
    }
  
    /* Split to three calls start, middle, and tail. Middle range is aligned */

    /* First Region */
    StartAddr = MemoryMap->PhysicalStart;
    RemainingSize = ROUND_UP_1MB (StartAddr) - StartAddr;

    if (RemainingSize)
    {
      DEBUG ((DEP_DEBUG_LEVEL, " S:%016lx - %016lx \n", StartAddr, StartAddr+RemainingSize));
      Status = Cpu->SetMemoryAttributes (Cpu, StartAddr, RemainingSize, EFI_MEMORY_XP);
      if (EFI_ERROR (Status))
        DEBUG(( EFI_D_WARN, "Error: Unable to set XP\n"));
    }

    /* Middle Region */
    StartAddr = ROUND_UP_1MB (MemoryMap->PhysicalStart);
    RemainingSize = EntrySize - RemainingSize;
    RemainingSize = RemainingSize & ~0xFFFFF; /* Align to 1 MB */

    if (RemainingSize)
    {
      DEBUG ((DEP_DEBUG_LEVEL, " M:%016lx - %016lx \n", StartAddr, StartAddr+RemainingSize));
      Status = Cpu->SetMemoryAttributes (Cpu, StartAddr, RemainingSize, EFI_MEMORY_XP);
      if (EFI_ERROR (Status))
        DEBUG(( EFI_D_WARN, "Error: Unable to set XP\n"));
    }

    /* Tail region */    
    StartAddr = StartAddr+RemainingSize;
    RemainingSize = EntrySize - (ROUND_UP_1MB (MemoryMap->PhysicalStart) - MemoryMap->PhysicalStart) - RemainingSize;
    if (RemainingSize)
    {
      DEBUG ((DEP_DEBUG_LEVEL, " T:%016lx - %016lx \n", StartAddr, StartAddr+RemainingSize));
      Status = Cpu->SetMemoryAttributes (Cpu, StartAddr, RemainingSize, EFI_MEMORY_XP);
      if (EFI_ERROR (Status))
        DEBUG(( EFI_D_WARN, "Error: Unable to set XP\n"));
    }

    MemoryMap = NEXT_MEMORY_DESCRIPTOR (MemoryMap, DescriptorSize);
  }

  DEBUG ((DEP_DEBUG_LEVEL, "\n----------------------------------------------------------------------------\n"));
}

STATIC
VOID
Dep64Handler (VOID)
{
  EFI_STATUS    Status = EFI_UNSUPPORTED;
  MemRegionInfo MemRegion;
	
 /* Look for "Exception Vectors" memory region in config file */
  Status = GetMemRegionInfoByName("CPU Vectors", &MemRegion);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "ERROR: CPU Vectors region not present in cfg file !\n"));
    return;
  }
  /* PCD is needed by ArmPkg, so ensure PCD and config file entry is in sync */
  ASSERT (FixedPcdGet64(PcdCpuVectorBaseAddress) == MemRegion.MemBase);

  /* Set RO for CPU Vectors */
  Status = ArmSetMemoryRegionReadOnly (MemRegion.MemBase, MemRegion.MemSize);
  if (Status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "RO: 0x%016lx--0x%016lx [Failed]\n", MemRegion.MemBase,  MemRegion.MemSize));
    return;
  }
}

VOID
CpuProtocolInstalledNotification (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
#ifdef ENABLE_DEP_64	
  Dep64Handler();
#else
  DEBUG ((EFI_D_ERROR, "Warning: DEP 64 disabled\n"));
#ifndef DISABLE_DEP
  Dep32Handler();
#endif  
#endif  
}

VOID
SplashScreenRenderCallback (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  EFI_STATUS                          Status                = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_UTILS_PROTOCOL    *pDisplayUtilsProtocol = NULL;
  EFI_DISPLAY_UTILS_RENDERLOGO_CONFIG RenderLogoCfg         = {0};

  /* Render the splash logo */
  Status = gBS->LocateProtocol(&gQcomDisplayUtilsProtocolGuid, NULL, (void**)&pDisplayUtilsProtocol);
  if ((EFI_SUCCESS != Status) || (NULL == pDisplayUtilsProtocol))
  {
    DEBUG((EFI_D_WARN, "EnvDxe locate display utils protocol failed with status: %r\n", Status));
  }
  else
  {
    RenderLogoCfg.uFlags |= DISPLAY_UTILS_RENDERLOGO_RENDER_SPLASH_SCREEN;
    if (EFI_SUCCESS != (Status = pDisplayUtilsProtocol->DisplayUtilsRenderLogo(&RenderLogoCfg)))
    {
      DEBUG((EFI_D_WARN, "EnvDxe render splash screen failed with status: %r\r\n", Status));
    }
  }

}

VOID
RefreshBlockIoHandles (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  EFI_STATUS        Status = EFI_NO_MEDIA;
  UINT32            Attributes = 0, MaxHandles = 0;
  UINTN             i = 0;
  HandleInfo        HandleInfoList[60];
  PartiSelectFilter HandleFilter;

  DEBUG((EFI_D_ERROR, "Refreshing BlockIO Handles\n"));

  Attributes |= BLK_IO_SEL_PARTITIONED_GPT;
  Attributes |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attributes |= BLK_IO_SEL_SELECT_ROOT_DEVICE_ONLY;
  
  HandleFilter.PartitionType = 0;
  HandleFilter.RootDeviceType = NULL;
  HandleFilter.VolumeName = 0;

  MaxHandles = sizeof(HandleInfoList)/sizeof(*HandleInfoList);

  Status = GetBlkIOHandles(Attributes, &HandleFilter, HandleInfoList, &MaxHandles);

  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR, "Could not get BlkIO Handles, Status=0x%x\n", Status));
    return;
  }

  if ((MaxHandles == 0) || (HandleInfoList == NULL))
  {
    DEBUG((EFI_D_ERROR, "No handles found\n"));
    return;
  }

  for (i=0; i<MaxHandles; i++)
  {
    Status = gBS->DisconnectController (HandleInfoList[i].Handle, NULL, NULL);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "Could not disconnect BlkIO Handles\n"));
      return;
    }
    
    Status = gBS->ConnectController (HandleInfoList[i].Handle, NULL, NULL, TRUE);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "Could not reconnect BlkIO Handles\n"));
      return;
    }
  }
  return;
}

EFI_STATUS
RegisterSplashScreenRendering (VOID)
{
  EFI_STATUS Status;

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               SplashScreenRenderCallback,
                               NULL,
                               &gDisplaySetModeSignalGuid,
                               &GraphicsProtocolNotificationEvent);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
RegisterBlockIoRefresh (VOID)
{
  EFI_STATUS Status;

  Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, 
                              TPL_CALLBACK, 
                              RefreshBlockIoHandles, 
                              NULL, 
                              &gBlockIoRefreshGuid,
                              &BlockIoRefreshEvent);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
RegisterDEP (VOID)
{
  EFI_STATUS Status;

  // Set up to be notified when the Cpu protocol is installed.
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, CpuProtocolInstalledNotification, NULL, &CpuProtocolNotificationEvent);    
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (&gEfiCpuArchProtocolGuid, CpuProtocolNotificationEvent, (VOID *)&CpuProtocolNotificationToken);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
EFI_GetRamPartitionVersion (
   IN EFI_RAMPARTITION_PROTOCOL *This,
   OUT UINT32                   *MajorVersion,
   OUT UINT32                   *MinorVersion
   )
{
  if ((MajorVersion == NULL) || (MinorVersion == NULL) )
    return EFI_INVALID_PARAMETER;

  *MajorVersion = 2;
  *MinorVersion = 0;

  return EFI_SUCCESS; 
}

EFI_STATUS
EFI_GetHighestBankBit (
   IN EFI_RAMPARTITION_PROTOCOL *This,
   OUT UINT32                   *HighestBankBit
   )
{
  if (HighestBankBit == NULL)
    return EFI_INVALID_PARAMETER;

  *HighestBankBit = 0;

#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}

EFI_STATUS
EFI_GetMinPasrSize (
   IN EFI_RAMPARTITION_PROTOCOL *This,
   IN OUT UINT32                *MinPasrSize
   )
{
  EFI_STATUS    Status;
  if (MinPasrSize == NULL)
    return EFI_INVALID_PARAMETER;

  Status = GetMinPasrSize(MinPasrSize);
  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  if (*MinPasrSize == 0)
    return EFI_UNSUPPORTED;
  
  return EFI_SUCCESS;

}


EFI_STATUS
EFI_GetRamPartitions (
   IN EFI_RAMPARTITION_PROTOCOL *This,
   OUT RamPartitionEntry        *RamPartitions,
   IN OUT UINT32                *NumPartition
   )
{
  EFI_STATUS    Status;
  MemRegionInfo *pRamPartTable = NULL;
  UINTN         AvailNumParts = 0;
  UINTN         i             = 0;

  if (NumPartition == NULL)
    return EFI_INVALID_PARAMETER;

  Status = GetRamPartitions(&AvailNumParts, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    if (AvailNumParts > (UINTN)(*NumPartition))
    {
      *NumPartition = (UINT32)AvailNumParts;
      return EFI_BUFFER_TOO_SMALL;
    }

    pRamPartTable = AllocatePool (sizeof(MemRegionInfo) * AvailNumParts);
    if (pRamPartTable == NULL)
      return EFI_OUT_OF_RESOURCES;

    Status = GetRamPartitions(&AvailNumParts, pRamPartTable);
    if (Status != EFI_SUCCESS)
      return EFI_UNSUPPORTED;

    if (pRamPartTable == NULL)
      return EFI_UNSUPPORTED;

    for (i=0; i<AvailNumParts; i++)
    {
      RamPartitionEntry *CurrentEntry;

      CurrentEntry = &RamPartitions[i];

      CurrentEntry->Base            = pRamPartTable[i].MemBase;
      CurrentEntry->AvailableLength = pRamPartTable[i].MemSize;
    }
  }

  if (pRamPartTable)
    FreePool (pRamPartTable);
  return EFI_SUCCESS;
}

EFI_STATUS
EFI_GetInstalledPhysicalMemory (
   IN EFI_RAMPARTITION_PROTOCOL *This,
   IN OUT UINTN                 *MemoryCapacity
   )
{
  EFI_STATUS    Status;
  if (MemoryCapacity == NULL)
    return EFI_INVALID_PARAMETER;

  Status = GetInstalledPhysicalMemory(MemoryCapacity);
  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  if (*MemoryCapacity == 0)
    return EFI_UNSUPPORTED;
  
  return EFI_SUCCESS;

}

STATIC EFI_RAMPARTITION_PROTOCOL RamPartitionProtocol = 
{
  EFI_RAMPARTITION_PROTOCOL_REVISION,
  EFI_GetRamPartitionVersion,
  EFI_GetHighestBankBit,
  EFI_GetRamPartitions,
  EFI_GetMinPasrSize,
  EFI_GetInstalledPhysicalMemory
};


EFI_STATUS
SetupRamPartitionProtocol ()
{
  EFI_STATUS Status;
  STATIC EFI_HANDLE ImageHandle;

  Status = InitRamPartitionTableLib();
  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;

  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, 
                                                   &gEfiRamPartitionProtocolGuid, 
                                                   &RamPartitionProtocol, 
                                                   NULL, NULL);
  return Status;
}

/* Add Version information to system table */
EFI_STATUS
FwVerToSystemTable (VOID)
{
  EFI_HOB_GUID_TYPE *GuidHob;
  UINT32            *FwVerHex;
    
  GuidHob = GetFirstGuidHob(&gQcomFwVersionHexGuid);
  if (GuidHob == NULL)
  {
    DEBUG((EFI_D_INFO, "FW version hex HOB not found\n"));
    return EFI_NOT_FOUND;
  }

  FwVerHex = GET_GUID_HOB_DATA(GuidHob);
  if (FwVerHex == NULL)
  {
    DEBUG((EFI_D_INFO, "FW Version Hex not found in HOB\n"));
    return EFI_NOT_FOUND;
  }

  gST->FirmwareRevision = *FwVerHex;

  return EFI_SUCCESS;
}



EFI_STATUS
EFI_SetDdrThroughputLow (
   IN EFI_QCOMENV_PROTOCOL *This
   )
{
  EFI_STATUS                 Status;
  UINT32                     DdrMinimumFreq = 200000;
  EFI_DDRGETINFO_PROTOCOL   *pDDRGetInfoProtocol = NULL;

  Status = gBS->LocateProtocol(&gEfiDDRGetInfoProtocolGuid, NULL, (VOID**)&pDDRGetInfoProtocol);
  if (EFI_ERROR(Status) || (pDDRGetInfoProtocol == NULL))
  {
    DEBUG((EFI_D_ERROR, "Locate EFI_DDRGETINFO_PROTOCOL failed, Status =  (0x%x)\r\n", Status));
    return Status;
  }

  Status = pDDRGetInfoProtocol->SetDDRFreq(pDDRGetInfoProtocol, DdrMinimumFreq);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_WARN, "Failed to set DDR Freq"));
  }

  return Status;
}


EFI_STATUS SetDdrThroughputMax ( VOID )
{
  EFI_STATUS                 Status              = EFI_SUCCESS;
  UINT32                     DdrMaxEnabledFreq   = 0;
  EFI_DDRGETINFO_PROTOCOL   *pDDRGetInfoProtocol = NULL;

  Status = gBS->LocateProtocol(&gEfiDDRGetInfoProtocolGuid, NULL, (VOID**)&pDDRGetInfoProtocol);
  if (EFI_ERROR(Status) || (pDDRGetInfoProtocol == NULL))
  {
    /* EfiCreateProtocolNotifyEvent : If there are no instances of ProtocolGuid in the handle 
    database at the time this function is invoked, then the notification function is still 
    executed one time; So ignore LocateProtocol error */
    DEBUG((EFI_D_INFO, "Locate EFI_DDRGETINFO_PROTOCOL failed, Status =  (0x%x)\r\n", Status));
    return Status;
  }

  Status = pDDRGetInfoProtocol->GetDDRMaxEnabledFreq(pDDRGetInfoProtocol, &DdrMaxEnabledFreq);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_WARN, "Failed to get DDR Max Enabled Freq"));
    return Status;
  }

  Status = pDDRGetInfoProtocol->SetDDRFreq(pDDRGetInfoProtocol, DdrMaxEnabledFreq);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_WARN, "Failed to set DDR Freq"));
  }

  return Status;
}

/**
 LPM callback; Reduces the DDR throughput to low when entering LPM and restores back when exiting LPM
*/
EFI_STATUS EFIAPI EnvDxeLPMCb (
    IN EFI_STATUS_CODE_TYPE     CodeType,
    IN EFI_STATUS_CODE_VALUE    Value,
    IN UINT32                   Instance,
    IN EFI_GUID                 *CallerId,
    IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
    )
{
  EFI_STATUS          status       = EFI_UNSUPPORTED;
  BOOLEAN             bMatch       = FALSE;
  LPM_EVENT_EXT_DATA *LpmEventData = NULL;

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == PcdGet32 (PcdLpm)) && 
      (Data != NULL))
  {
    /* Map LpmEventData */
    LpmEventData = (LPM_EVENT_EXT_DATA*)( Data + 1);

    bMatch = CompareGuid(&(LpmEventData->CalleeGuid), &gQcomDDRLpmGuid);
    if(TRUE == bMatch)
    {
      switch (LpmEventData->LPMState)
      {
        case LPM_ENTRY:
          status = EFI_SetDdrThroughputLow (NULL);
        break;

        case LPM_EXIT:
          status = SetDdrThroughputMax ();
        break;

        default:
          DEBUG(( EFI_D_WARN, "EnvDxeLPMCb: Invalid LPM state %d \r\n", LpmEventData->LPMState));
        break;
      }
      /* Error log */
      if (status != EFI_SUCCESS)
      {
      DEBUG(( EFI_D_ERROR, "EnvDxeLPMCb: LPM state = %d Error %r\r\n", LpmEventData->LPMState, status));
      }
    }
  }  
  
  return status;
}

EFI_STATUS EnvDxeRegisterLPMCb( VOID )
{
  EFI_STATUS               Status               = EFI_SUCCESS;
  EFI_RSC_HANDLER_PROTOCOL *mRscHandlerProtocol = NULL;

  Status = gBS->LocateProtocol(&gEfiRscHandlerProtocolGuid, NULL, (VOID **) &mRscHandlerProtocol);
  
  if((EFI_SUCCESS == Status) && (mRscHandlerProtocol != NULL))
  {
    Status = mRscHandlerProtocol->Register(EnvDxeLPMCb, TPL_CALLBACK);
  }

  return Status;
}

/**
  This is the Event notification function for DDR Info Protocol

  @param    Event   Pointer to this event
  @param    Context Event handler private data 
 **/
VOID
EFIAPI
NotifyDDRInfoCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                 Status;

  if(EFI_SUCCESS == SetDdrThroughputMax())
  {
    Status = EnvDxeRegisterLPMCb();
    if (EFI_ERROR (Status))
      DEBUG(( EFI_D_WARN, "Error: Unable to register LPM Callback \n"));
  }
}

/**
 Register callback for DDRInfo Protocol notify
*/
EFI_STATUS
RegisterDDRInfoNotify (VOID)
{
  EFI_EVENT  DDRInfoNotifyEvent    = (EFI_EVENT)NULL;
  VOID      *DDRInfoNotifyEventReg = NULL;

  DDRInfoNotifyEvent = EfiCreateProtocolNotifyEvent (
                          &gEfiDDRGetInfoProtocolGuid,
                          TPL_CALLBACK,
                          NotifyDDRInfoCallback,
                          NULL,
                          &DDRInfoNotifyEventReg
                          );

  if (DDRInfoNotifyEvent == NULL)
    return EFI_DEVICE_ERROR;
  else
    return EFI_SUCCESS;
}

STATIC EFI_QCOMENV_PROTOCOL QcomEnvProtocol = 
{
  EFI_QCOMENV_PROTOCOL_REVISION,
  EFI_SetDdrThroughputLow
};


EFI_STATUS
SetupQcomEnvProtocol ()
{
  EFI_STATUS Status;
  STATIC EFI_HANDLE ImageHandle;
  
  Status = RegisterDDRInfoNotify();
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "Error: Unable to register NPA notify event callback\n"));
    return Status; 
  }
  // QcomEnvProtocol Depends upon RegisterDDRInfoNotify for initial
  // DDR freq bump up vote.
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, 
                                                   &gEfiQcomEnvProtocolGuid, 
                                                   &QcomEnvProtocol, 
                                                   NULL, NULL);
  return Status;
}



/**
  EnvDxe Entry point. Registers to handle FBPT update events, and gST to InfoBlk

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       All handlers installed

**/

EFI_STATUS
EFIAPI
EnvDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;

  /* Lot of modules here depend on InfoBlock */
  InfoBlkPtr = (UefiInfoBlkType*)GetInfoBlkPtr();
  ASSERT (InfoBlkPtr != NULL);

  AddDxeSymbolInfo ();

  Status = UpdateInfoBlkSystemTableAddr();
  if (EFI_ERROR (Status))
    DEBUG(( EFI_D_WARN, "Error: Unable to update InfoBlk with gST \n"));

  Status = InitRuntimeDbg();
  if (EFI_ERROR (Status))
    DEBUG(( EFI_D_WARN, "Error: Unable to init runtime debug table \n"));

  Status = InitMmioRuntime();
  if (EFI_ERROR (Status))
    DEBUG(( EFI_D_WARN, "Error: Unable to convert memory to runtime \n"));

  Status = RegisterSerialLogFlush();
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to register for serial flush timer callback \n"));

  Status = SetupRamPartitionProtocol();
  if (EFI_ERROR (Status))
    DEBUG(( EFI_D_WARN, "Error: Unable to install RamPartition protocol \n"));

  Status = FwVerToSystemTable();
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to set firmware version in System Table \n"));

  Status = RegisterDEP();
  if (EFI_ERROR (Status))
   DEBUG(( EFI_D_WARN, "Error: Failed DEP registration \n"));
  
  Status = RegisterBlockIoRefresh();
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to register BlkIo refresh callback\n"));

  Status = RegisterSplashScreenRendering();
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to register splash screen rendering callback\n"));

  Status = TzRegisterModeSwitchProtocol();
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to register 32-bit mode switch protocol\n"));

  /* Register the event handling function to reclaim variable for OS usage. */
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             ReadyToBootCallBack,
             NULL,
             &ReadyToBootEvent
             );
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Error: Unable to register RTB event callback\n"));


  Status = SetupQcomEnvProtocol();
  if (EFI_ERROR (Status))
    DEBUG(( EFI_D_WARN, "Error: Unable to install QcomEnv protocol \n"));

  return EFI_SUCCESS;
}

