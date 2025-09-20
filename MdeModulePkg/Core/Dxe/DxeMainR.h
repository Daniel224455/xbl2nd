/** @file
  The internal header file includes the common header files, defines
  internal structure and functions used by DxeCore module.

Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DXE_MAINR_H_
#define _DXE_MAINR_H_


/* Services with API's utilizing Locks */
extern EFI_BOOT_SERVICES mBootServicesLk;
extern EFI_DXE_SERVICES mDxeServicesLk;

EFI_TPL
EFIAPI
CoreRaiseTplR (
  IN EFI_TPL      NewTpl
  );

VOID
EFIAPI
CoreRestoreTplR (
  IN EFI_TPL NewTpl
  );

EFI_STATUS
EFIAPI
CoreAllocatePagesR (
  IN  EFI_ALLOCATE_TYPE     Type,
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 NumberOfPages,
  OUT EFI_PHYSICAL_ADDRESS  *Memory
  );

EFI_STATUS
EFIAPI
CoreFreePagesR (
  IN EFI_PHYSICAL_ADDRESS  Memory,
  IN UINTN                 NumberOfPages
  );

EFI_STATUS
EFIAPI
CoreGetMemoryMapR (
  IN OUT UINTN                  *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  OUT UINTN                     *MapKey,
  OUT UINTN                     *DescriptorSize,
  OUT UINT32                    *DescriptorVersion
  );

EFI_STATUS
EFIAPI
CoreAllocatePoolR (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            Size,
  OUT VOID            **Buffer
  );

EFI_STATUS
EFIAPI
CoreFreePoolR (
  IN VOID  *Buffer
  );

EFI_STATUS
EFIAPI
CoreCreateEventR (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN VOID                     *NotifyContext, OPTIONAL
  OUT EFI_EVENT               *Event
  );

EFI_STATUS
EFIAPI
CoreSetTimerR (
  IN EFI_EVENT            UserEvent,
  IN EFI_TIMER_DELAY      Type,
  IN UINT64               TriggerTime
  );

EFI_STATUS
EFIAPI
CoreWaitForEventR (
  IN UINTN        NumberOfEvents,
  IN EFI_EVENT    *UserEvents,
  OUT UINTN       *UserIndex
  );

EFI_STATUS
EFIAPI
CoreSignalEventR (
  IN EFI_EVENT    UserEvent
  );

EFI_STATUS
EFIAPI
CoreCloseEventR (
  IN EFI_EVENT    UserEvent
  );

EFI_STATUS
EFIAPI
CoreCheckEventR (
  IN EFI_EVENT        UserEvent
  );

EFI_STATUS
EFIAPI
CoreInstallProtocolInterfaceR (
  IN OUT EFI_HANDLE     *UserHandle,
  IN EFI_GUID           *Protocol,
  IN EFI_INTERFACE_TYPE InterfaceType,
  IN VOID               *Interface
  );

EFI_STATUS
EFIAPI
CoreReinstallProtocolInterfaceR (
  IN EFI_HANDLE     UserHandle,
  IN EFI_GUID       *Protocol,
  IN VOID           *OldInterface,
  IN VOID           *NewInterface
  );

EFI_STATUS
EFIAPI
CoreUninstallProtocolInterfaceR (
  IN EFI_HANDLE       UserHandle,
  IN EFI_GUID         *Protocol,
  IN VOID             *Interface
  );

EFI_STATUS
EFIAPI
CoreHandleProtocolR (
  IN EFI_HANDLE       UserHandle,
  IN EFI_GUID         *Protocol,
  OUT VOID            **Interface
  );

EFI_STATUS
EFIAPI
CoreRegisterProtocolNotifyR (
  IN EFI_GUID       *Protocol,
  IN EFI_EVENT      Event,
  OUT  VOID         **Registration
  );

EFI_STATUS
EFIAPI
CoreLocateHandleR (
  IN EFI_LOCATE_SEARCH_TYPE   SearchType,
  IN EFI_GUID                 *Protocol   OPTIONAL,
  IN VOID                     *SearchKey  OPTIONAL,
  IN OUT UINTN                *BufferSize,
  OUT EFI_HANDLE              *Buffer
  );

EFI_STATUS
EFIAPI
CoreLocateDevicePathR (
  IN EFI_GUID                       *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL   **DevicePath,
  OUT EFI_HANDLE                    *Device
  );

EFI_STATUS
EFIAPI
CoreInstallConfigurationTableR (
  IN EFI_GUID *Guid,
  IN VOID     *Table
  );

EFI_STATUS
EFIAPI
CoreLoadImageR (
  IN BOOLEAN                    BootPolicy,
  IN EFI_HANDLE                 ParentImageHandle,
  IN EFI_DEVICE_PATH_PROTOCOL   *FilePath,
  IN VOID                       *SourceBuffer   OPTIONAL,
  IN UINTN                      SourceSize,
  OUT EFI_HANDLE                *ImageHandle
  );

EFI_STATUS
EFIAPI
CoreStartImageR (
  IN EFI_HANDLE  ImageHandle,
  OUT UINTN      *ExitDataSize,
  OUT CHAR16     **ExitData  OPTIONAL
  );

EFI_STATUS
EFIAPI
CoreExitR (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  Status,
  IN UINTN       ExitDataSize,
  IN CHAR16      *ExitData  OPTIONAL
  );

EFI_STATUS
EFIAPI
CoreUnloadImageR (
  IN EFI_HANDLE  ImageHandle
  );

EFI_STATUS
EFIAPI
CoreExitBootServicesR (
  IN EFI_HANDLE   ImageHandle,
  IN UINTN        MapKey
  );

EFI_STATUS
EFIAPI
CoreStallR (
  IN UINTN            Microseconds
  );

EFI_STATUS
EFIAPI
CoreSetWatchdogTimerR (
  IN UINTN    Timeout,
  IN UINT64   WatchdogCode,
  IN UINTN    DataSize,
  IN CHAR16   *WatchdogData OPTIONAL
  );

EFI_STATUS
EFIAPI
CoreConnectControllerR (
  IN  EFI_HANDLE                ControllerHandle,
  IN  EFI_HANDLE                *DriverImageHandle    OPTIONAL,
  IN  EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath  OPTIONAL,
  IN  BOOLEAN                   Recursive
  );

EFI_STATUS
EFIAPI
CoreDisconnectControllerR (
  IN  EFI_HANDLE  ControllerHandle,
  IN  EFI_HANDLE  DriverImageHandle  OPTIONAL,
  IN  EFI_HANDLE  ChildHandle        OPTIONAL
  );

EFI_STATUS
EFIAPI
CoreOpenProtocolR (
  IN  EFI_HANDLE                UserHandle,
  IN  EFI_GUID                  *Protocol,
  OUT VOID                      **Interface OPTIONAL,
  IN  EFI_HANDLE                ImageHandle,
  IN  EFI_HANDLE                ControllerHandle,
  IN  UINT32                    Attributes
  );

EFI_STATUS
EFIAPI
CoreCloseProtocolR (
  IN  EFI_HANDLE                UserHandle,
  IN  EFI_GUID                  *Protocol,
  IN  EFI_HANDLE                AgentHandle,
  IN  EFI_HANDLE                ControllerHandle
  );

EFI_STATUS
EFIAPI
CoreOpenProtocolInformationR (
  IN  EFI_HANDLE                          UserHandle,
  IN  EFI_GUID                            *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN                               *EntryCount
  );

EFI_STATUS
EFIAPI
CoreProtocolsPerHandleR (
  IN EFI_HANDLE       UserHandle,
  OUT EFI_GUID        ***ProtocolBuffer,
  OUT UINTN           *ProtocolBufferCount
  );

EFI_STATUS
EFIAPI
CoreLocateHandleBufferR (
  IN EFI_LOCATE_SEARCH_TYPE       SearchType,
  IN EFI_GUID                     *Protocol OPTIONAL,
  IN VOID                         *SearchKey OPTIONAL,
  IN OUT UINTN                    *NumberHandles,
  OUT EFI_HANDLE                  **Buffer
  );


EFI_STATUS
EFIAPI
CoreLocateProtocolR (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  );

EFI_STATUS
EFIAPI
CoreInstallMultipleProtocolInterfacesV (
  IN OUT EFI_HANDLE           *Handle,
  IN VA_LIST                   Args
  );

EFI_STATUS
EFIAPI
CoreInstallMultipleProtocolInterfacesR (
  IN OUT EFI_HANDLE           *Handle,
  ...
  );


EFI_STATUS
EFIAPI
CoreUninstallMultipleProtocolInterfacesV (
  IN EFI_HANDLE           Handle,
  IN VA_LIST              Args
  );

EFI_STATUS
EFIAPI
CoreUninstallMultipleProtocolInterfacesR (
  IN EFI_HANDLE           Handle,
  ...
  );

EFI_STATUS
EFIAPI
CoreCreateEventExR (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN CONST VOID               *NotifyContext, OPTIONAL
  IN CONST EFI_GUID           *EventGroup,    OPTIONAL
  OUT EFI_EVENT               *Event
  );


EFI_STATUS
EFIAPI
CoreAddMemorySpaceR (
  IN EFI_GCD_MEMORY_TYPE   GcdMemoryType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  );

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
  );

EFI_STATUS
EFIAPI
CoreFreeMemorySpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

EFI_STATUS
EFIAPI
CoreRemoveMemorySpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

EFI_STATUS
EFIAPI
CoreGetMemorySpaceDescriptorR (
  IN  EFI_PHYSICAL_ADDRESS             BaseAddress,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *Descriptor
  );


EFI_STATUS
EFIAPI
CoreSetMemorySpaceAttributesR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Attributes
  );

EFI_STATUS
EFIAPI
CoreGetMemorySpaceMapR (
  OUT UINTN                            *NumberOfDescriptors,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  **MemorySpaceMap
  );

EFI_STATUS
EFIAPI
CoreAddIoSpaceR (
  IN EFI_GCD_IO_TYPE       GcdIoType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

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
  );

EFI_STATUS
EFIAPI
CoreFreeIoSpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

EFI_STATUS
EFIAPI
CoreRemoveIoSpaceR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

EFI_STATUS
EFIAPI
CoreGetIoSpaceDescriptorR (
  IN  EFI_PHYSICAL_ADDRESS         BaseAddress,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  *Descriptor
  );

EFI_STATUS
EFIAPI
CoreGetIoSpaceMapR (
  OUT UINTN                        *NumberOfDescriptors,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  **IoSpaceMap
  );

EFI_STATUS
EFIAPI
CoreDispatcherR (
  VOID
  );

EFI_STATUS
EFIAPI
CoreScheduleR (
  IN  EFI_HANDLE  FirmwareVolumeHandle,
  IN  EFI_GUID    *DriverName
  );

EFI_STATUS
EFIAPI
CoreTrustR (
  IN  EFI_HANDLE  FirmwareVolumeHandle,
  IN  EFI_GUID    *DriverName
  );

EFI_STATUS
EFIAPI
CoreProcessFirmwareVolumeR (
  IN VOID                             *FvHeader,
  IN UINTN                            Size,
  OUT EFI_HANDLE                      *FVProtocolHandle
  );

EFI_STATUS
EFIAPI
CoreSetMemorySpaceCapabilitiesR (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  );

#endif
