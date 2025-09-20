/*
                    ShmBridgeDxe.c

   Implementation of the Shared Memory BridgeEFI protocol interface

  Copyright (c) 2017-2020 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Confidential and Proprietary,
*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who     what, where, why
  ----------  ---     -----------------------------------------------------------
  2020-06-09  ps      Change TGCM error label from warn to Info
  2019-12-02  mj      Avoid sending originalTpl value as 0 to RestoreTpl
  2019-05-09  dp      Changes to Allocate SHMBridge after 4K offset of TGCM
  2018-12-24  pr      To allocate common Shmbridge from TrEE carveout memory
  2018-09-15  pr      Raised TPL level at allocation and free of Shmbridge nodes
  2018-05-03  pranavj Ported SHMBridge changes to Poipu
  2017-06-15          Initial revision.

============================================================================*/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/ShmBridgeLib.h>
#include <Protocol/EFIShmBridge.h>
#include <Library/DxeServicesTableLib.h>

// SHMBridge enabling flag
//#define UEFI_SHMBRIDGE_ENABLE
// Default 10M buffer allocation
#define DEFAULT_ALLOCATION_SIZE 0xA00000
#define TGCM_TPM_ADDRESS_OFFSET 1024

typedef struct
{
    VOID              *pAddr;
    UINTN              uSize;
    SHMBRIDGE_TYPE     uAppType;
    BOOLEAN            uAllocated;
    LIST_ENTRY         Link;
} SHMBRIDGE_NODE;

EFI_EVENT  ExitBootServicesEvent;

STATIC LIST_ENTRY pShmBridgeList = INITIALIZE_LIST_HEAD_VARIABLE (pShmBridgeList);
STATIC VOID *pUncachedMem = NULL;
STATIC VOID *pCurrUncachedMem = NULL;
STATIC UINT32 uUncachedMemSize = 0;
STATIC UINTN uHandle;
STATIC BOOLEAN  UseTgcm = FALSE;

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
    MemRegionInfo TgcmRegInfo;
    CHAR8* TgcmLable = "TGCM";

    Status = GetConfigValue ("ShmBridgeMemSize", &uUncachedMemSize);
    if (Status != EFI_SUCCESS || uUncachedMemSize < DEFAULT_ALLOCATION_SIZE) {
        uUncachedMemSize = DEFAULT_ALLOCATION_SIZE;
        Status = EFI_SUCCESS;
    }

    DEBUG((EFI_D_INFO, "ShmBridgeInitialize: GetConfigValue status %d\n", Status));
    // Enable SHmBridge
#ifdef UEFI_SHMBRIDGE_ENABLE
    Status = EnableShmBridge();
    DEBUG((EFI_D_ERROR, "ShmBridgeInitialize: enable status %d\n", Status));
#endif

    //Zero Out the structure
    SetMem(&TgcmRegInfo, sizeof(MemRegionInfo), 0);

    // Read memory regions info from CFG file
    Status = GetMemRegionInfoByName(TgcmLable, &TgcmRegInfo);
    DEBUG((EFI_D_INFO, "ShmBridgeInitialize: GetMemRegionInfoByName status %d\n", Status));
    if (Status == EFI_SUCCESS)
    {
        pUncachedMem = (VOID *)(((UINT32*)TgcmRegInfo.MemBase) + TGCM_TPM_ADDRESS_OFFSET);
        UseTgcm = TRUE;
    }
    else
    {
        DEBUG((EFI_D_INFO, "Failed to read the TGCM region details from configuration.\r\n"));
    }

    if (UseTgcm == FALSE)
    {
        // Allocate ShmBridge
        pUncachedMem =  UncachedAllocatePool(uUncachedMemSize);
        if (pUncachedMem == NULL)
        {
            uUncachedMemSize = 0;
            return EFI_OUT_OF_RESOURCES;
        }
    }

    SetMem(pUncachedMem, (UINTN)uUncachedMemSize, 0);
    DEBUG((EFI_D_INFO, "ShmBridgeInitialize: addr %p size %x\n", pUncachedMem, uUncachedMemSize));

    // Create ShmBridge with TZ
    Status = CreateShmBridge(pUncachedMem, uUncachedMemSize, &uHandle);
    if (Status != EFI_SUCCESS) {
        if (UseTgcm == FALSE)
        {
            UncachedFreePool(pUncachedMem);
        }
        uUncachedMemSize = 0;
        DEBUG((EFI_D_ERROR, "ShmBridgeInitialize: ShmBrige creation failed, Status %d\n", Status));
        return EFI_DEVICE_ERROR;
    }
    DEBUG((EFI_D_INFO, "ShmBridge with addr %p size %x & handle %x\n", pUncachedMem, uUncachedMemSize, uHandle));

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
    EFI_TPL OriginalTpl = 0;

    OriginalTpl = gBS->RaiseTPL(TPL_CALLBACK);

    // validate request size
    if (uReqSize == 0) {
        DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: request size (%x) invalid.\n", uReqSize));
        goto Exit;
    }

    // validate app type
    if (uAppType <= ShmBridgeDefault ||
            uAppType > ShmBridgeRunTimeSvcData) {
        DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: app type invalid., %u\n", uAppType));
        goto Exit;
    }

    // Rounded up for multiple 4k
    uReqPages = uReqSize / MEM_ALIGNMENT_SIZE;
    if ((uReqSize - uReqPages * MEM_ALIGNMENT_SIZE) != 0) {
        uReqSize = (uReqPages + 1) * MEM_ALIGNMENT_SIZE;
    }

    if (pUncachedMem == NULL) {
        DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: Global Shmbridge pointer is invalid\n"));
        goto Exit;
    }

    // Check if a free node fits
    pNode =  CheckShmBridgeFreeNode(uReqSize);
    if ((pNode != NULL)) {
        pNode->uAllocated = TRUE;
        pAddr = pNode->pAddr;
    }
    // Allocate new node
    else {
        if ((pCurrUncachedMem + uReqSize) < (pUncachedMem + uUncachedMemSize)) {
            Status = AddShmBridgeNode(pCurrUncachedMem, uReqSize, uAppType);
            if (Status != EFI_SUCCESS) {
                DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: add node failed, status %d.\n", Status));
                goto Exit;
            }
            pAddr = pCurrUncachedMem;

            // Advance to next available memory space
            pCurrUncachedMem += uReqSize;
        }
        else {
            DEBUG((EFI_D_ERROR, "ShmBridgeAllocate: out of resource error.\n"));
            goto Exit;
        }
    }

    if ((pAddr != NULL)) {
        pAddr = ZeroMem(pAddr, uReqSize);
    }
    *uSize = uReqSize;
Exit:
    gBS->RestoreTPL(OriginalTpl);
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
    EFI_TPL OriginalTpl = 0;

    if (pAddr == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    if (pUncachedMem == NULL) {
        DEBUG((EFI_D_ERROR, "ShmBridgeFree: Global Shmbridge pointer is invalid\n"));
        return EFI_INVALID_PARAMETER;
    }

    OriginalTpl = gBS->RaiseTPL(TPL_CALLBACK);

    // ToDo: Rama --> Merge adjacent nodes if they are free to reduce length of linked list

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

    gBS->RestoreTPL(OriginalTpl);
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

    if (UseTgcm == FALSE)
    {
        if (pUncachedMem != NULL)
        {
            UncachedFreePool(pUncachedMem);
        }
    }
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


    // Initialize Global Shmbridge here
    Status = ShmBridgeInitialize();
    if (EFI_SUCCESS != Status) {
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

