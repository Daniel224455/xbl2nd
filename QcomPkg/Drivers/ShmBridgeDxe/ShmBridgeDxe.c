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
// SHMBridge enabling flag
//#define UEFI_SHMBRIDGE_ENABLE
// Default 10M buffer allocation
#define DEFAULT_ALLOCATION_SIZE 0xA00000

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
STATIC LIST_ENTRY pShmBridgeList = INITIALIZE_LIST_HEAD_VARIABLE (pShmBridgeList);
STATIC VOID *pUncachedMem = NULL;
STATIC VOID *pCurrUncachedMem = NULL;
STATIC UINT32 uUncachedMemSize = 0;
STATIC UINTN uHandle;

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

STATIC 
EFI_STATUS
ShmBridgeCreate(
  IN  EFI_SHMBRIDGE_PROTOCOL *This, 
  IN  MapAddrInfo            *pMapping,
  IN  DstVMPermInfo          *pDestVMList,
  IN  UINT32                  uNumDestinations,
  IN  UINT32                  uPermissions,
  OUT UINTN                  *uHandle
);

STATIC 
EFI_STATUS
ShmBridgeDelete(
    IN  EFI_SHMBRIDGE_PROTOCOL *This, 
    IN UINTN                    uHandle
);

static EFI_SHMBRIDGE_PROTOCOL ShmBridgeProtocol =
{
    EFI_SHMBRIDGE_PROTOCOL_REVISION,
    ShmBridgeAllocate,
    ShmBridgeFree,
    ShmBridgeCreate,
    ShmBridgeDelete
};

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
/**
 * Function to add a shared memory bridge 
 * 
 * @param pAddr 
 * @param uSize 
 * @param uAppType 
 * 
 * @return STATIC EFI_STATUS 
 */
STATIC EFI_STATUS
AddShmBridgeNode(
    IN VOID            *pAddr,
    IN UINTN            uSize,
    IN SHMBRIDGE_TYPE   uAppType
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    SHMBRIDGE_NODE *pNewNode;

    // Add a node
    pNewNode = AllocatePool(sizeof(SHMBRIDGE_NODE));

    // Cannot allocate new node
    if (pNewNode == NULL)
    {
        DEBUG((EFI_D_ERROR, "AddShmBridgeNode: out of resource error.\n"));
        ASSERT (FALSE);

        return EFI_OUT_OF_RESOURCES;
    }

    // Add node
    pNewNode->pAddr       = pAddr;
    pNewNode->uSize       = uSize;
    pNewNode->uAppType    = uAppType;
    pNewNode->uAllocated  = TRUE;

    InsertTailList(&pShmBridgeList, &pNewNode->Link);

    return Status;
}

/**
 * ShmBridge Initilization 
 * 
 * @return STATIC EFI_STATUS 
 */
STATIC EFI_STATUS
ShmBridgeInitialize(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = GetConfigValue ("ShmBridgeMemSize", &uUncachedMemSize);
    if (Status != EFI_SUCCESS || uUncachedMemSize < DEFAULT_ALLOCATION_SIZE) {
        uUncachedMemSize = DEFAULT_ALLOCATION_SIZE;
    }
    // Enable SHmBridge
#ifdef UEFI_SHMBRIDGE_ENABLE
    Status = EnableShmBridge();
    DEBUG((EFI_D_ERROR, "ShmBridgeInitialize: enable status %d\n", Status));
#endif 
    // Allocate ShmBridge
    pUncachedMem = UncachedAllocatePool(uUncachedMemSize);
    if (pUncachedMem == NULL) {
        uUncachedMemSize = 0;
        return EFI_OUT_OF_RESOURCES;
    }
    DEBUG((EFI_D_INFO, "ShmBridgeInitialize: addr %p size %x\n", pUncachedMem, uUncachedMemSize));

    // Create ShmBridge with TZ
    Status = CreateShmBridge(pUncachedMem, uUncachedMemSize, &uHandle);
    if (Status != EFI_SUCCESS) {
        UncachedFreePool(pUncachedMem);
        uUncachedMemSize = 0;
        DEBUG((EFI_D_ERROR, "ShmBridgeInitialize: ShmBrige creation failed, Status %d\n", Status));
        return EFI_DEVICE_ERROR;
    }
    DEBUG((EFI_D_INFO, "ShmBridgeInitialize: %p %x %x\n", pUncachedMem, uUncachedMemSize, uHandle));

    pCurrUncachedMem = pUncachedMem;

    return Status;
}

/**
 * Function to check if an unallocated node is available 
 * 
 * @param uSize 
 * 
 * @return SHMBRIDGE_NODE 
 */
STATIC SHMBRIDGE_NODE*
CheckShmBridgeFreeNode(
    IN UINTN uSize
    )
{
    SHMBRIDGE_NODE *pNode = NULL;
    SHMBRIDGE_NODE *pNewNode = NULL;
    LIST_ENTRY      *Link;

    for (Link = pShmBridgeList.ForwardLink; Link != &pShmBridgeList; Link = Link->ForwardLink)
    {
        pNode = BASE_CR (Link, SHMBRIDGE_NODE, Link);
        if (pNode->uAllocated == FALSE) {
            if (pNode->uSize == uSize) {
                // Found a free node
                return pNode;
            }
            else {
                if (pNode->uSize > uSize) {
                    if (pNewNode == NULL) {
                        pNewNode = pNode;
                    }
                    else {
                        if (pNewNode->uSize > pNode->uSize) {
                            pNewNode = pNode;
                        }
                    }
                }
            }
        }
    }

    return pNewNode;
}

STATIC
VOID *
ShmBridgeAllocate(
  IN  EFI_SHMBRIDGE_PROTOCOL *This, 
  IN  UINTN                  *uSize,
  IN  SHMBRIDGE_TYPE          uAppType
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    SHMBRIDGE_NODE *pNode = NULL;
    VOID *pAddr = NULL;
    UINTN uReqSize = *uSize;
    UINTN uReqPages;

    // validate request size
    if (uReqSize == 0){
        DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: request size (%x) invalid.\n", uReqSize));
        return NULL;
    }
  
    // validate app type
    if (uAppType <= ShmBridgeDefault ||
        uAppType > ShmBridgeRunTimeSvcData) {
        DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: app type invalid., %u\n", uAppType));
        return NULL;
    }

    // Rounded up for multiple 4k
    uReqPages = uReqSize/MEM_ALIGNMENT_SIZE;
    if((uReqSize - uReqPages*MEM_ALIGNMENT_SIZE) != 0) {
        uReqSize = (uReqPages + 1)*MEM_ALIGNMENT_SIZE;
    }

    // Initialize if not already done
    if (pUncachedMem == NULL) {
        Status = ShmBridgeInitialize();
        if (EFI_SUCCESS != Status) {
            return NULL;
        }
    }

    // Check if a free node fits
    pNode =  CheckShmBridgeFreeNode(uReqSize);
    if ((pNode != NULL)) {
        pNode->uAllocated = TRUE;
        pAddr = pNode->pAddr;
    }
    // Allocate new node
    else {
        if ((pCurrUncachedMem+uReqSize)<(pUncachedMem+uUncachedMemSize)) {
            Status = AddShmBridgeNode(pCurrUncachedMem, uReqSize, uAppType);
            if (Status != EFI_SUCCESS) {
                DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: add node failed, status %d.\n", Status));
                return NULL;
            }
            pAddr = pCurrUncachedMem;

            // Advance to next available memory space
            pCurrUncachedMem += uReqSize;
        }
        else {
            DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: out of resource error.\n"));
            return NULL;
        }
    }
  
    if ((pAddr != NULL)) {
        pAddr = ZeroMem(pAddr, uReqSize);
    }
    *uSize = uReqSize;

    return pAddr;
}


static EFI_STATUS
ShmBridgeFree(
  IN EFI_SHMBRIDGE_PROTOCOL *This, 
  IN VOID                   *pAddr
)
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    SHMBRIDGE_NODE *pNode = NULL;
    LIST_ENTRY     *Link;

    if (pAddr == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    for (Link = pShmBridgeList.ForwardLink; Link != &pShmBridgeList; Link = Link->ForwardLink)
    {
        pNode = BASE_CR (Link, SHMBRIDGE_NODE, Link);

        // Found the node
        if ((pNode->pAddr == pAddr))
        {
            // set the node to alocated FALSE
            pNode->uAllocated = FALSE;
            Status = EFI_SUCCESS;
            break;
        }
    }

    return Status; 
}

STATIC
EFI_STATUS
ShmBridgeCreate(
  IN  EFI_SHMBRIDGE_PROTOCOL *This, 
  IN  MapAddrInfo            *pMapping,
  IN  DstVMPermInfo          *pDestVMList,
  IN  UINT32                  uNumDestinations,
  IN  UINT32                  uPermissions,
  OUT UINTN                  *uHandle
)
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;

    if ((pMapping == NULL || pDestVMList == NULL || pMapping->uSize == 0) ||
        (pMapping->uSize & MEM_ALIGNMENT_MASK) || uHandle == NULL)
    {
        return Status;
    }

    Status = CreateGenericShmBridge((ShmMapAddrInfo_t*)pMapping, (ShmDstVMPermInfo_t*)pDestVMList, uNumDestinations, uPermissions, uHandle);

    return Status;
}

STATIC
EFI_STATUS
ShmBridgeDelete(
  IN  EFI_SHMBRIDGE_PROTOCOL *This, 
  IN  UINTN                   uHandle
)
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;

    Status = DeleteShmBridge(uHandle);

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
    EFI_STATUS Status = EFI_SUCCESS;
    SHMBRIDGE_NODE *pOldNode;
    LIST_ENTRY     *Link;
    
    // Test if the list is empty
    Link = pShmBridgeList.ForwardLink;
    if (Link == &pShmBridgeList) {
        return;
    }

    // Free all the pages and nodes
    do {
        pOldNode = BASE_CR (Link, SHMBRIDGE_NODE, Link);
        // Point to the next entry
        Link = Link->ForwardLink;

      RemoveEntryList (&pOldNode->Link);
      FreePool (pOldNode);
    } while (Link != &pShmBridgeList);    

    //  destroy the shared memory bridge at EFI stage from TZ
    Status = DeleteShmBridge(uHandle);
    UncachedFreePool(pUncachedMem);
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

  return Status;
}

