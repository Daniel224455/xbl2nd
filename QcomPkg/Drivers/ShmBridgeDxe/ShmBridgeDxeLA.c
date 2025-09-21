/*============================================================================
  FILE:         ShmBridgeDxe.c

  OVERVIEW:     Implementation of the Shared Memory BridgeEFI protocol interface

  DEPENDENCIES: None

                Copyright (c) 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary,
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-15       Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/ShmBridgeLib.h>
#include <Protocol/EFIShmBridge.h>

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
// Default 10M buffer allocation
#define DEFAULT_ALLOCATION_SIZE 0x500000

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
typedef struct
{
  VOID              *pAddr;
  UINTN              uSize;
  SHMBRIDGE_TYPE     uAppType;
  BOOLEAN            uAllocated;
  LIST_ENTRY         Link;
} SHMBRIDGE_NODE;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
EFI_EVENT  ExitBootServicesEvent;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

STATIC
VOID *
ShmBridgeAllocate(
    IN  EFI_SHMBRIDGE_PROTOCOL *This,
    IN OUT  UINTN              *uSize,
    IN  SHMBRIDGE_TYPE          uAppType
);

STATIC
EFI_STATUS
ShmBridgeFree(
    IN EFI_SHMBRIDGE_PROTOCOL *This,
    IN VOID                   *pAddr
);

static EFI_SHMBRIDGE_PROTOCOL ShmBridgeProtocol =
{
    EFI_SHMBRIDGE_PROTOCOL_REVISION,
    ShmBridgeAllocate,
    ShmBridgeFree
};

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
STATIC
VOID *
ShmBridgeAllocate(
  IN  EFI_SHMBRIDGE_PROTOCOL *This,
  IN  UINTN                  *uSize,
  IN  SHMBRIDGE_TYPE          uAppType
)
{
    VOID *pAddr = NULL;

    pAddr = UncachedAllocateAlignedZeroPool(*uSize, EFI_PAGE_SIZE);

    return pAddr;
}


static EFI_STATUS
ShmBridgeFree(
  IN EFI_SHMBRIDGE_PROTOCOL *This,
  IN VOID                   *pAddr
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UncachedFreeAlignedPool(pAddr);

    return Status;
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
/**
  This is called when notified that boot services has exited

  @param  IN      Event          The event to be signaled.

**/
VOID EFIAPI
ShmBridgeExitBootServicesHandler(
  IN EFI_EVENT Event,
  IN VOID *Context
)
{
        return;
}

EFI_STATUS
EFIAPI
ShmBridgeEntryPoint(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_HANDLE handle = NULL;
    EFI_STATUS Status;

    // Install protocol
    Status = gBS->InstallMultipleProtocolInterfaces(&handle,
                                                    &gEfiShmBridgeProtocolGuid,
                                                    (void **)&ShmBridgeProtocol,
                                                    NULL,
                                                    NULL,
                                                    NULL);
    if (EFI_ERROR (Status))
    {
        DEBUG(( EFI_D_ERROR, "Failed to install ShmBridgeProtocol, Status = (0x%p)\r\n", Status));
        return Status;
    }
#if 0
    // Register event
    Status = gBS->CreateEventEx (
                                EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                ShmBridgeExitBootServicesHandler,
                                NULL,
                                &gEfiEventExitBootServicesGuid,
                                &ExitBootServicesEvent
                                );
    if (EFI_ERROR (Status))
    {
        DEBUG(( EFI_D_ERROR, "Failed to register ExitBootServcies event, Status = (0x%p)\r\n", Status));
        return Status;
    }
#endif
  return Status;
}

