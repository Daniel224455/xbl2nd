/** @file
  DXE Core Main Entry Point

Copyright (c) 2011-2014, 2016, 2019 Qualcomm Technologies, Inc. All rights reserved.
Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "DxeMain.h"
#include "Event.h"

extern EFI_EVENT       gYieldEvent;


//
// DXE Core Module Variables
//
// Any of these structure changes/updates in mainline DxeMain.c should be brought here
EFI_BOOT_SERVICES mBootServicesLk = {
  {
    EFI_BOOT_SERVICES_SIGNATURE,                                                          // Signature
    EFI_BOOT_SERVICES_REVISION,                                                           // Revision
    sizeof (EFI_BOOT_SERVICES),                                                           // HeaderSize
    0,                                                                                    // CRC32
    0                                                                                     // Reserved
  },
  (EFI_RAISE_TPL)                               CoreRaiseTplR,                             // RaiseTPL
  (EFI_RESTORE_TPL)                             CoreRestoreTplR,                           // RestoreTPL
  (EFI_ALLOCATE_PAGES)                          CoreAllocatePagesR,                        // AllocatePages
  (EFI_FREE_PAGES)                              CoreFreePagesR,                            // FreePages
  (EFI_GET_MEMORY_MAP)                          CoreGetMemoryMapR,                         // GetMemoryMap
  (EFI_ALLOCATE_POOL)                           CoreAllocatePoolR,                         // AllocatePool
  (EFI_FREE_POOL)                               CoreFreePoolR,                             // FreePool
  (EFI_CREATE_EVENT)                            CoreCreateEventR,                          // CreateEvent
  (EFI_SET_TIMER)                               CoreSetTimerR,                             // SetTimer
  (EFI_WAIT_FOR_EVENT)                          CoreWaitForEventR,                         // WaitForEvent
  (EFI_SIGNAL_EVENT)                            CoreSignalEventR,                          // SignalEvent
  (EFI_CLOSE_EVENT)                             CoreCloseEventR,                           // CloseEvent
  (EFI_CHECK_EVENT)                             CoreCheckEventR,                           // CheckEvent
  (EFI_INSTALL_PROTOCOL_INTERFACE)              CoreInstallProtocolInterfaceR,             // InstallProtocolInterface
  (EFI_REINSTALL_PROTOCOL_INTERFACE)            CoreReinstallProtocolInterfaceR,           // ReinstallProtocolInterface
  (EFI_UNINSTALL_PROTOCOL_INTERFACE)            CoreUninstallProtocolInterfaceR,           // UninstallProtocolInterface
  (EFI_HANDLE_PROTOCOL)                         CoreHandleProtocolR,                       // HandleProtocol
  (VOID *)                                      NULL,                                     // Reserved
  (EFI_REGISTER_PROTOCOL_NOTIFY)                CoreRegisterProtocolNotifyR,               // RegisterProtocolNotify
  (EFI_LOCATE_HANDLE)                           CoreLocateHandleR,                         // LocateHandle
  (EFI_LOCATE_DEVICE_PATH)                      CoreLocateDevicePathR,                     // LocateDevicePath
  (EFI_INSTALL_CONFIGURATION_TABLE)             CoreInstallConfigurationTableR,            // InstallConfigurationTable
  (EFI_IMAGE_LOAD)                              CoreLoadImageR,                            // LoadImage
  (EFI_IMAGE_START)                             CoreStartImageR,                           // StartImage
  (EFI_EXIT)                                    CoreExitR,                                 // Exit
  (EFI_IMAGE_UNLOAD)                            CoreUnloadImageR,                          // UnloadImage
  (EFI_EXIT_BOOT_SERVICES)                      CoreExitBootServicesR,                     // ExitBootServices
  (EFI_GET_NEXT_MONOTONIC_COUNT)                CoreEfiNotAvailableYetArg1,               // GetNextMonotonicCount
  (EFI_STALL)                                   CoreStallR,                                // Stall
  (EFI_SET_WATCHDOG_TIMER)                      CoreSetWatchdogTimerR,                     // SetWatchdogTimer
  (EFI_CONNECT_CONTROLLER)                      CoreConnectControllerR,                    // ConnectController
  (EFI_DISCONNECT_CONTROLLER)                   CoreDisconnectControllerR,                 // DisconnectController
  (EFI_OPEN_PROTOCOL)                           CoreOpenProtocolR,                         // OpenProtocol
  (EFI_CLOSE_PROTOCOL)                          CoreCloseProtocolR,                        // CloseProtocol
  (EFI_OPEN_PROTOCOL_INFORMATION)               CoreOpenProtocolInformationR,              // OpenProtocolInformation
  (EFI_PROTOCOLS_PER_HANDLE)                    CoreProtocolsPerHandleR,                   // ProtocolsPerHandle
  (EFI_LOCATE_HANDLE_BUFFER)                    CoreLocateHandleBufferR,                   // LocateHandleBuffer
  (EFI_LOCATE_PROTOCOL)                         CoreLocateProtocolR,                       // LocateProtocol
  (EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)    CoreInstallMultipleProtocolInterfaces,    // InstallMultipleProtocolInterfaces
  (EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)  CoreUninstallMultipleProtocolInterfaces,  // UninstallMultipleProtocolInterfaces
  (EFI_CALCULATE_CRC32)                         CoreEfiNotAvailableYetArg3,               // CalculateCrc32
  (EFI_COPY_MEM)                                CopyMem,                                  // CopyMem
  (EFI_SET_MEM)                                 SetMem,                                   // SetMem
  (EFI_CREATE_EVENT_EX)                         CoreCreateEventExR                         // CreateEventEx
};

EFI_DXE_SERVICES mDxeServicesLk = {
  {
    DXE_SERVICES_SIGNATURE,                                           // Signature
    DXE_SERVICES_REVISION,                                            // Revision
    sizeof (DXE_SERVICES),                                            // HeaderSize
    0,                                                                    // CRC32
    0                                                                     // Reserved
  },
  (EFI_ADD_MEMORY_SPACE)             CoreAddMemorySpaceR,                  // AddMemorySpace
  (EFI_ALLOCATE_MEMORY_SPACE)        CoreAllocateMemorySpaceR,             // AllocateMemorySpace
  (EFI_FREE_MEMORY_SPACE)            CoreFreeMemorySpaceR,                 // FreeMemorySpace
  (EFI_REMOVE_MEMORY_SPACE)          CoreRemoveMemorySpaceR,               // RemoveMemorySpace
  (EFI_GET_MEMORY_SPACE_DESCRIPTOR)  CoreGetMemorySpaceDescriptorR,        // GetMemorySpaceDescriptor
  (EFI_SET_MEMORY_SPACE_ATTRIBUTES)  CoreSetMemorySpaceAttributesR,        // SetMemorySpaceAttributes
  (EFI_GET_MEMORY_SPACE_MAP)         CoreGetMemorySpaceMapR,               // GetMemorySpaceMap
  (EFI_ADD_IO_SPACE)                 CoreAddIoSpaceR,                      // AddIoSpace
  (EFI_ALLOCATE_IO_SPACE)            CoreAllocateIoSpaceR,                 // AllocateIoSpace
  (EFI_FREE_IO_SPACE)                CoreFreeIoSpaceR,                     // FreeIoSpace
  (EFI_REMOVE_IO_SPACE)              CoreRemoveIoSpaceR,                   // RemoveIoSpace
  (EFI_GET_IO_SPACE_DESCRIPTOR)      CoreGetIoSpaceDescriptorR,            // GetIoSpaceDescriptor
  (EFI_GET_IO_SPACE_MAP)             CoreGetIoSpaceMapR,                   // GetIoSpaceMap
  (EFI_DISPATCH)                     CoreDispatcherR,                      // Dispatch
  (EFI_SCHEDULE)                     CoreScheduleR,                        // Schedule
  (EFI_TRUST)                        CoreTrustR,                           // Trust
  (EFI_PROCESS_FIRMWARE_VOLUME)      CoreProcessFirmwareVolumeR,           // ProcessFirmwareVolume
  (EFI_SET_MEMORY_SPACE_CAPABILITIES)CoreSetMemorySpaceCapabilitiesR,      // SetMemorySpaceCapabilities
};

VOID *DxeLockPtr;
VOID DxeLockInit (VOID)
{
  DxeLockPtr = InitRecursiveLock ("DxeLock");
}

VOID DxeLock (VOID)
{
  Lock (DxeLockPtr);
}

VOID DxeUnlock (VOID)
{
  UnLock (DxeLockPtr);
}

EFI_TPL
EFIAPI
CoreRaiseTplInternal (
  IN EFI_TPL      NewTpl
  );


VOID
EFIAPI
CoreRestoreTplInternal (
  IN EFI_TPL NewTpl
  );

#ifdef ENABLE_TPL_LOCK_DEBUG

#define RAISE_OVERHEAD_THRESHOLD     16

#define MAX_TPL_LOG                  32

volatile UINTN TplError = 0;
UINTN TplCnt;
UINT32 TplHist[MAX_TPL_LOG];

UINTN  MaxRaiseThreshold;
#endif

UINTN  RaiseCnt, RestoreCnt;

EFI_TPL
EFIAPI
CoreRaiseTplR (
  IN EFI_TPL      NewTpl
  )
{
  EFI_STATUS            Status;

#ifdef ENABLE_TPL_LOCK_DEBUG
  UINTN                 TempThr;

  if (RaiseCnt > (RAISE_OVERHEAD_THRESHOLD + RestoreCnt)) 
    while (1);

  /* This should be positive all the time, if restore is done more than
   * Raise ever, unlocking is wrong */
  TempThr = RaiseCnt - RestoreCnt;
  if (MaxRaiseThreshold < TempThr)
    MaxRaiseThreshold = TempThr;
#endif

  /* This lock spans through RestoreTPL if Fine Grained Locking is not enabled */
  DxeLock ();

  ++RaiseCnt;

#ifdef ENABLE_TPL_LOCK_DEBUG
  {
    UINT32 TLog;

    TLog = (1 << 15) | (gEfiCurrentTpl << 8) | (NewTpl) | ((0xFFFF & (UINTN)get_current_thread()) << 16);

    while (TplError);

    TplCnt++;

    TplHist[TplCnt & (MAX_TPL_LOG - 1)] = TLog;

    while (TplError);
  }
#endif

  Status = CoreRaiseTpl(NewTpl);
  
#ifdef ENABLE_FINE_GRAINED_LOCKING
  DxeUnlock ();
#endif

  return Status;
}

VOID
EFIAPI
CoreRestoreTplR (
  IN EFI_TPL NewTpl
  )
{
#ifdef ENABLE_FINE_GRAINED_LOCKING
  DxeLock ();
#endif

  ++RestoreCnt;

#ifdef ENABLE_TPL_LOCK_DEBUG
  {
    UINT32 TLog;

    TLog = (gEfiCurrentTpl << 8) | (NewTpl) | ((0xFFFF & (UINTN)get_current_thread()) << 16);

    while (TplError);

    TplCnt++;

    while (TplError);

    TplHist[TplCnt & (MAX_TPL_LOG - 1)] = TLog;
  }
#endif

  CoreRestoreTpl(NewTpl);
  
  DxeUnlock ();
}

EFI_STATUS
EFIAPI
CoreAllocatePagesR (
  IN  EFI_ALLOCATE_TYPE     Type,
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 NumberOfPages,
  OUT EFI_PHYSICAL_ADDRESS  *Memory
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreAllocatePages (Type, MemoryType, NumberOfPages, Memory);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreFreePagesR (
  IN EFI_PHYSICAL_ADDRESS  Memory,
  IN UINTN                 NumberOfPages
  )
{
  EFI_STATUS            Status;
  DxeLock ();

  Status = CoreFreePages (Memory, NumberOfPages);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreGetMemoryMapR (
  IN OUT UINTN                  *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  OUT UINTN                     *MapKey,
  OUT UINTN                     *DescriptorSize,
  OUT UINT32                    *DescriptorVersion
  )
{
  EFI_STATUS            Status;


  DxeLock ();

  Status = CoreGetMemoryMap (MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreAllocatePoolR (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            Size,
  OUT VOID            **Buffer
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreAllocatePool (PoolType, Size, Buffer);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreFreePoolR (
  IN VOID  *Buffer
  )
{
  EFI_STATUS            Status;


  DxeLock ();

  Status = CoreFreePool (Buffer);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreCreateEventR (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN VOID                     *NotifyContext, OPTIONAL
  OUT EFI_EVENT               *Event
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreCreateEvent (Type, NotifyTpl, NotifyFunction, NotifyContext, Event);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreSetTimerR (
  IN EFI_EVENT            UserEvent,
  IN EFI_TIMER_DELAY      Type,
  IN UINT64               TriggerTime
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreSetTimer (UserEvent, Type, TriggerTime);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreWaitForEventR (
  IN UINTN        NumberOfEvents,
  IN EFI_EVENT    *UserEvents,
  OUT UINTN       *UserIndex
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreWaitForEvent (NumberOfEvents, UserEvents, UserIndex);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreSignalEventR (
  IN EFI_EVENT    UserEvent
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreSignalEvent (UserEvent);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreCloseEventR (
  IN EFI_EVENT    UserEvent
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreCloseEvent (UserEvent);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreCheckEventR (
  IN EFI_EVENT        UserEvent
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreCheckEvent (UserEvent);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreInstallProtocolInterfaceR (
  IN OUT EFI_HANDLE     *UserHandle,
  IN EFI_GUID           *Protocol,
  IN EFI_INTERFACE_TYPE InterfaceType,
  IN VOID               *Interface
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreInstallProtocolInterface (UserHandle, Protocol, InterfaceType, Interface);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreReinstallProtocolInterfaceR (
  IN EFI_HANDLE     UserHandle,
  IN EFI_GUID       *Protocol,
  IN VOID           *OldInterface,
  IN VOID           *NewInterface
  )
{
  EFI_STATUS                Status;
  DxeLock ();

  Status = CoreReinstallProtocolInterface (UserHandle, Protocol, OldInterface, NewInterface);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreUninstallProtocolInterfaceR (
  IN EFI_HANDLE       UserHandle,
  IN EFI_GUID         *Protocol,
  IN VOID             *Interface
  )
{
  EFI_STATUS            Status;
  DxeLock ();

  Status = CoreUninstallProtocolInterface (UserHandle, Protocol, Interface);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreHandleProtocolR (
  IN EFI_HANDLE       UserHandle,
  IN EFI_GUID         *Protocol,
  OUT VOID            **Interface
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreHandleProtocol (UserHandle, Protocol, Interface);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreRegisterProtocolNotifyR (
  IN EFI_GUID       *Protocol,
  IN EFI_EVENT      Event,
  OUT  VOID         **Registration
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreRegisterProtocolNotify (Protocol, Event, Registration);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreLocateHandleR (
  IN EFI_LOCATE_SEARCH_TYPE   SearchType,
  IN EFI_GUID                 *Protocol   OPTIONAL,
  IN VOID                     *SearchKey  OPTIONAL,
  IN OUT UINTN                *BufferSize,
  OUT EFI_HANDLE              *Buffer
  )
{
  EFI_STATUS          Status;
  DxeLock ();

  Status = CoreLocateHandle (SearchType, Protocol, SearchKey, BufferSize, Buffer);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreLocateDevicePathR (
  IN EFI_GUID                       *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL   **DevicePath,
  OUT EFI_HANDLE                    *Device
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreLocateDevicePath (Protocol, DevicePath, Device);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreInstallConfigurationTableR (
  IN EFI_GUID *Guid,
  IN VOID     *Table
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreInstallConfigurationTable (Guid, Table);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreLoadImageR (
  IN BOOLEAN                    BootPolicy,
  IN EFI_HANDLE                 ParentImageHandle,
  IN EFI_DEVICE_PATH_PROTOCOL   *FilePath,
  IN VOID                       *SourceBuffer   OPTIONAL,
  IN UINTN                      SourceSize,
  OUT EFI_HANDLE                *ImageHandle
  )
{
  EFI_STATUS    Status;
  DxeLock ();

  Status = CoreLoadImage (BootPolicy, ParentImageHandle, FilePath, SourceBuffer, SourceSize, ImageHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreStartImageR (
  IN EFI_HANDLE  ImageHandle,
  OUT UINTN      *ExitDataSize,
  OUT CHAR16     **ExitData  OPTIONAL
  )
{
  EFI_STATUS                    Status;

  DxeLock ();

  Status = CoreStartImage (ImageHandle, ExitDataSize, ExitData);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreExitR (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  Status,
  IN UINTN       ExitDataSize,
  IN CHAR16      *ExitData  OPTIONAL
  )
{
  DxeLock ();

  Status = CoreExit (ImageHandle, Status, ExitDataSize, ExitData);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreUnloadImageR (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS                 Status;
  DxeLock ();

  Status = CoreUnloadImage (ImageHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreExitBootServicesR (
  IN EFI_HANDLE   ImageHandle,
  IN UINTN        MapKey
  )
{
  EFI_STATUS                Status;
  DxeLock ();

  Status = CoreExitBootServices (ImageHandle, MapKey);

  /* Do not release lock, since we will be unloading all images from memory
   * and would have potentially removed the image that can get unblocked
   * after this.
   */
  // DxeUnlock ();
  
  gYieldEvent = NULL;

  return Status;
}

EFI_STATUS
EFIAPI
CoreStallR (
  IN UINTN            Microseconds
  )
{
  EFI_STATUS            Status;
  static UINT64 AccumulatedTime;

  DxeLock ();

  /* See if the following logic belongs here or need to find a proper home */
  if (Microseconds < 1000)
    AccumulatedTime += Microseconds;

  if ((AccumulatedTime >= 1000) || (Microseconds >= 1000))
  {
    AccumulatedTime = 0;
    CoreSignalEvent (gYieldEvent);
  }
  
  Status = CoreStall (Microseconds);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreSetWatchdogTimerR (
  IN UINTN    Timeout,
  IN UINT64   WatchdogCode,
  IN UINTN    DataSize,
  IN CHAR16   *WatchdogData OPTIONAL
  )
{
  EFI_STATUS  Status;
  DxeLock ();

  Status = CoreSetWatchdogTimer (Timeout, WatchdogCode, DataSize, WatchdogData);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreConnectControllerR (
  IN  EFI_HANDLE                ControllerHandle,
  IN  EFI_HANDLE                *DriverImageHandle    OPTIONAL,
  IN  EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath  OPTIONAL,
  IN  BOOLEAN                   Recursive
  )
{
  EFI_STATUS                           Status;
  DxeLock ();

  Status = CoreConnectController (ControllerHandle, DriverImageHandle, RemainingDevicePath, Recursive);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreDisconnectControllerR (
  IN  EFI_HANDLE  ControllerHandle,
  IN  EFI_HANDLE  DriverImageHandle  OPTIONAL,
  IN  EFI_HANDLE  ChildHandle        OPTIONAL
  )
{
  EFI_STATUS                          Status;
  DxeLock ();

  Status = CoreDisconnectController (ControllerHandle, DriverImageHandle, ChildHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreOpenProtocolR (
  IN  EFI_HANDLE                UserHandle,
  IN  EFI_GUID                  *Protocol,
  OUT VOID                      **Interface OPTIONAL,
  IN  EFI_HANDLE                ImageHandle,
  IN  EFI_HANDLE                ControllerHandle,
  IN  UINT32                    Attributes
  )
{
  EFI_STATUS          Status;

  DxeLock ();

  Status = CoreOpenProtocol (UserHandle, Protocol, Interface, ImageHandle, ControllerHandle, Attributes);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreCloseProtocolR (
  IN  EFI_HANDLE                UserHandle,
  IN  EFI_GUID                  *Protocol,
  IN  EFI_HANDLE                AgentHandle,
  IN  EFI_HANDLE                ControllerHandle
  )
{
  EFI_STATUS          Status;
  DxeLock ();

  Status = CoreCloseProtocol (UserHandle, Protocol, AgentHandle, ControllerHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreOpenProtocolInformationR (
  IN  EFI_HANDLE                          UserHandle,
  IN  EFI_GUID                            *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN                               *EntryCount
  )
{
  EFI_STATUS                          Status;
  DxeLock ();

  Status = CoreOpenProtocolInformation (UserHandle, Protocol, EntryBuffer, EntryCount);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreProtocolsPerHandleR (
  IN EFI_HANDLE       UserHandle,
  OUT EFI_GUID        ***ProtocolBuffer,
  OUT UINTN           *ProtocolBufferCount
  )
{
  EFI_STATUS                          Status;
  DxeLock ();

  Status = CoreProtocolsPerHandle (UserHandle, ProtocolBuffer, ProtocolBufferCount);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreLocateHandleBufferR (
  IN EFI_LOCATE_SEARCH_TYPE       SearchType,
  IN EFI_GUID                     *Protocol OPTIONAL,
  IN VOID                         *SearchKey OPTIONAL,
  IN OUT UINTN                    *NumberHandles,
  OUT EFI_HANDLE                  **Buffer
  )
{
  EFI_STATUS          Status;
  DxeLock ();

  Status = CoreLocateHandleBuffer (SearchType, Protocol, SearchKey, NumberHandles, Buffer);
  
  DxeUnlock ();

  return Status;
}


EFI_STATUS
EFIAPI
CoreLocateProtocolR (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  )
{
  EFI_STATUS              Status;
  DxeLock ();

  Status = CoreLocateProtocol (Protocol, Registration, Interface);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreCreateEventExR (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN CONST VOID               *NotifyContext, OPTIONAL
  IN CONST EFI_GUID           *EventGroup,    OPTIONAL
  OUT EFI_EVENT               *Event
  )
{
  EFI_STATUS            Status;
  DxeLock ();

  Status = CoreCreateEventEx (Type, NotifyTpl, NotifyFunction, NotifyContext, EventGroup, Event);
  
  DxeUnlock ();

  return Status;
}


EFI_STATUS
EFIAPI
CoreAddMemorySpaceR (
  IN EFI_GCD_MEMORY_TYPE   GcdMemoryType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  )
{
  EFI_STATUS            Status;
  DxeLock ();

  Status = CoreAddMemorySpace (GcdMemoryType, BaseAddress, Length, Capabilities);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreAllocateMemorySpaceR (
  IN     EFI_GCD_ALLOCATE_TYPE  GcdAllocateType,
  IN     EFI_GCD_MEMORY_TYPE    GcdMemoryType,
  IN     UINTN                  Alignment,
  IN     UINT64                 Length,
  IN OUT EFI_PHYSICAL_ADDRESS   *BaseAddress,
  IN     EFI_HANDLE             ImageHandle,
  IN     EFI_HANDLE             DeviceHandle OPTIONAL
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreAllocateMemorySpace (GcdAllocateType, GcdMemoryType, Alignment, Length, BaseAddress, ImageHandle, DeviceHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreFreeMemorySpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreFreeMemorySpace (BaseAddress, Length);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreRemoveMemorySpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreRemoveMemorySpace (BaseAddress, Length);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreGetMemorySpaceDescriptorR (
  IN  EFI_PHYSICAL_ADDRESS             BaseAddress,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *Descriptor
  )
{
  EFI_STATUS         Status;
  DxeLock ();

  Status = CoreGetMemorySpaceDescriptor (BaseAddress, Descriptor);
  
  DxeUnlock ();

  return Status;
}


EFI_STATUS
EFIAPI
CoreSetMemorySpaceAttributesR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Attributes
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreSetMemorySpaceAttributes (BaseAddress, Length, Attributes);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreGetMemorySpaceMapR (
  OUT UINTN                            *NumberOfDescriptors,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  **MemorySpaceMap
  )
{
  EFI_STATUS                       Status;
  DxeLock ();

  Status = CoreGetMemorySpaceMap (NumberOfDescriptors, MemorySpaceMap);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreAddIoSpaceR (
  IN EFI_GCD_IO_TYPE       GcdIoType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreAddIoSpace (GcdIoType, BaseAddress, Length);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreAllocateIoSpaceR (
  IN     EFI_GCD_ALLOCATE_TYPE  GcdAllocateType,
  IN     EFI_GCD_IO_TYPE        GcdIoType,
  IN     UINTN                  Alignment,
  IN     UINT64                 Length,
  IN OUT EFI_PHYSICAL_ADDRESS   *BaseAddress,
  IN     EFI_HANDLE             ImageHandle,
  IN     EFI_HANDLE             DeviceHandle OPTIONAL
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreAllocateIoSpace (GcdAllocateType, GcdIoType, Alignment, Length, BaseAddress, ImageHandle, DeviceHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreFreeIoSpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreFreeIoSpace (BaseAddress, Length);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreRemoveIoSpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreRemoveIoSpace (BaseAddress, Length);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreGetIoSpaceDescriptorR (
  IN  EFI_PHYSICAL_ADDRESS         BaseAddress,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  *Descriptor
  )
{
  EFI_STATUS         Status;
  DxeLock ();

  Status = CoreGetIoSpaceDescriptor (BaseAddress, Descriptor);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreGetIoSpaceMapR (
  OUT UINTN                        *NumberOfDescriptors,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  **IoSpaceMap
  )
{
  EFI_STATUS                   Status;
  DxeLock ();

  Status = CoreGetIoSpaceMap (NumberOfDescriptors, IoSpaceMap);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreDispatcherR (
  VOID
  )
{
  EFI_STATUS                      Status;
  DxeLock ();

  Status = CoreDispatcher ();
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreScheduleR (
  IN  EFI_HANDLE  FirmwareVolumeHandle,
  IN  EFI_GUID    *DriverName
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreSchedule (FirmwareVolumeHandle, DriverName);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreTrustR (
  IN  EFI_HANDLE  FirmwareVolumeHandle,
  IN  EFI_GUID    *DriverName
  )
{
  EFI_STATUS            Status;

  DxeLock ();

  Status = CoreTrust (FirmwareVolumeHandle, DriverName);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreProcessFirmwareVolumeR (
  IN VOID                             *FvHeader,
  IN UINTN                            Size,
  OUT EFI_HANDLE                      *FVProtocolHandle
  )
{
  EFI_STATUS  Status;
  DxeLock ();

  Status = CoreProcessFirmwareVolume (FvHeader, Size, FVProtocolHandle);
  
  DxeUnlock ();

  return Status;
}

EFI_STATUS
EFIAPI
CoreSetMemorySpaceCapabilitiesR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  )
{
  EFI_STATUS    Status;

  DxeLock ();

  Status = CoreSetMemorySpaceCapabilities (BaseAddress, Length, Capabilities);
  
  DxeUnlock ();

  return Status;
}

#ifdef ENABLE_FINE_GRAINED_LOCKING
#error Fine grained locking config is not supported
#endif

