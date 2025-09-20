/** 
 Copyright (c) 2017,2018 Qualcomm Technologies, Inc. All Rights
 Reserved.

**/
#include <Uefi.h>
#include <Library/ShmBridgeLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

#define MAX_NUM_SHMBRIDGE_PARAM 4
#define MAX_SHMBRIDGE_RET_PARAM 4
#define AARCH_BIT 30
#define VM_PERM_X 0x1
#define VM_PERM_W 0x2
#define VM_PERM_R 0x4
#define SELF_OWNER_BIT 1
#define SELF_OWNER_PERM_SHIFT 2
#define SET_BIT(num, bit) ((num) |  (1 << (bit)))
#define AC_VM_HLOS 3
#define PERM_BITS 3
#define MAXSHMVMs 4
#define SHM_NUM_VM_SHIFT 9
#define VM_BITS 16

typedef enum
{
  SMC_INTERRUPTED                   =  1,  /* SMC call was interrupted                */
  SMC_SUCCESS                       =  0,  /* Success, requested syscall was called   */
  SMC_ERR_UNKNOWN_SMC_ID            = -1,  /* ARM-defined error of 0xFFFFFFFF         */
  SMC_ERR_SYSCALL_FAILED            = -2,  /* Syscall function returned failure       */
  SMC_ERR_SYSCALL_NOT_AVAILABLE     = -3,  /* No function associated with syscall def */
  SMC_ERR_RESERVED_FIELD_MBZ        = -4,  /* SMC ID err: reserved field must be zero */
  SMC_ERR_NUM_ARGS_MISMATCH         = -5,  /* Num args does not match func definition */
  SMC_ERR_INDIRECT_PARAM_PTR_NOT_NS = -6,  /* Ind. param ptr doesnt point to ns mem   */
  SMC_ERR_BUF_LEN_NOT_FOUND         = -7,  /* No buffer len following buffer ptr      */
  SMC_ERR_BUF_PTR_NOT_NS            = -8,  /* Buffer ptr does not point to ns mem     */
  SMC_ERR_NO_MEMORY                 = -9,  /* Heap is out of space                    */
  SMC_ERR_PARAM_ID_MISMATCH         = -10, /* Incorrect Param ID from NS world        */  
  SMC_ERR_YOU_BROKE_IT              = -11, /* How could you?  (unused) */
  SMC_ERR_BUSY                      = -12, /* TZ is busy waiting for listener rsp     */

  /* ARMv8 SMC spec: Must return 0xFFFFFFFF for following errors */
  SMC_ERR_SYSCALL_NOT_SUPPORTED     = SMC_ERR_UNKNOWN_SMC_ID,
  SMC_ERR_AARCH64_NOT_SUPPORTED     = SMC_ERR_UNKNOWN_SMC_ID, 
} SmcErrnoType;

// 64bit SMC call version
extern SmcErrnoType tz_armv8_smc_call(UINT32 smc_id, UINT32 param_id, UINT64 parameters[4], UINT64 *trusted_os_id);

EFI_STATUS
EFIAPI
CreateGenericShmBridge(
    IN ShmMapAddrInfo_t   *pMapping,
    IN ShmDstVMPermInfo_t *pDestVMList,
    IN UINT32              uNumDestinations,
    IN UINT32              uPermissions,
    OUT UINTN             *uHandle
);

STATIC EFI_STATUS
ShmBridgeSysCall(
  IN  UINT32               SmcId,
  IN  UINT32               ParamId,
  IN  UINT64               Parameters[MAX_NUM_SHMBRIDGE_PARAM],
  OUT UINT64               Results[MAX_SHMBRIDGE_RET_PARAM],
  OUT UINT64              *TrustedOsId
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  SmcErrnoType          TzStatus;
  UINT64                InternalTrustedOsId;
  UINTN                 i, NumArguments;

  if(Parameters == NULL || Results == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
    goto ErrorExit;
  }

  NumArguments = ParamId & 0xF;

  if (NumArguments > MAX_NUM_SHMBRIDGE_PARAM)
  {
    Status = EFI_INVALID_PARAMETER;
    goto ErrorExit;
  }

  TzStatus = tz_armv8_smc_call(
    SET_BIT(SmcId, AARCH_BIT),
    ParamId,
    Parameters,
    &InternalTrustedOsId
    );

  if(TzStatus != SMC_SUCCESS)
  {
    DebugPrint(DEBUG_INFO, "ShmBridgeSysCall failed, TzStatus = (0x%x)\n", TzStatus);
    Results[0] = Parameters[0];
    Status = EFI_DEVICE_ERROR;

    if(TzStatus == SMC_ERR_UNKNOWN_SMC_ID)
    {
      Status = EFI_UNSUPPORTED;
    }
  }
  else
  {
    for(i = 0; i < MAX_SHMBRIDGE_RET_PARAM - 1; i++)
    {
      Results[i] = Parameters[i];
    }
    if(Results[0] != 0)
    {
      DebugPrint(DEBUG_INFO, "ACError : ShmBridgeSysCall failed, TzStatus = (0x%x)\n", Results[0]);
    }
  }

  if(TrustedOsId != NULL)
  {
    *TrustedOsId = InternalTrustedOsId;
  }

ErrorExit:
  return Status;
}


/**
 * SMC call to TZ to create a shared memory bridge 
 * 
 * @param pAddr 
 * @param uSize 
 * 
 * @return STATIC FIL_STATUS 
 */
STATIC EFI_STATUS
SmcCreateShmBridge(
    IN VOID* pAddr,
    IN UINTN uSize,
    OUT UINTN *uHandle 
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT64 Parameters[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    UINT64 Results[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    shm_bridge_create_req_t *pReq;
    UINT64 TrustedOsId;

    DebugPrint(DEBUG_INFO, "SmcCreateShmBridge: pAddr, %p %x\n", pAddr, (UINT64)pAddr);
    pReq = (shm_bridge_create_req_t *)Parameters;
    pReq->uPFNAndNSPermFlags = (UINT64)pAddr;
    pReq->uSizeAndFlags = uSize;
    if ((pReq->uPFNAndNSPermFlags & MEM_ALIGNMENT_MASK) || 
        (pReq->uSizeAndFlags & MEM_ALIGNMENT_MASK)) {
        return EFI_INVALID_PARAMETER;
    }

    // uPFNAndNSPermFlags      bits[11:0]     0 
    // uIPFNAndSecurePermFlags bits[11:0]     0x6 for TZ RW permission
    // uSizeAndFlags           bits[11:9]     0   self owner feature is enabled
    //                         bits[4:2]      0x7 for owner RWX permission
    //                         bit[1]         0x1 self owner bit
    // uNSVmids                               0        
    pReq->uPFNAndNSPermFlags = (UINT64)pAddr;
    pReq->uIPFNAndSecurePermFlags = (UINT64)pAddr | VM_PERM_R | VM_PERM_W;
    pReq->uSizeAndFlags = uSize | ((VM_PERM_R | VM_PERM_W)<<SELF_OWNER_PERM_SHIFT) | (1ULL << SELF_OWNER_BIT);
    pReq->uNSVmids = 0;
    DebugPrint(DEBUG_INFO, "SmcCreateShmBridge: pReq, %x %x %x %x\n", pReq->uPFNAndNSPermFlags,
                                                                      pReq->uIPFNAndSecurePermFlags,
                                                                      pReq->uSizeAndFlags,
                                                                      pReq->uNSVmids);
    TrustedOsId = 0;

    Status = ShmBridgeSysCall(
        SMC_ID_SHM_BRIDGE_CREATE,
        PARAM_ID_SHM_BRIDGE_CREATE,
        Parameters,
        Results,
        &TrustedOsId
        );

    if (uHandle != NULL) {
        *uHandle = Results[1];
    }

    return Status;
}

/**
 * SMC call to TZ to destroy a shared memory bridge 
 * 
 * @param pAddr 
 * @param uSize 
 * 
 * @return STATIC EFI_STATUS 
 */
STATIC EFI_STATUS
SmcDestroyShmBridge(
    IN UINT64 uHandle
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT64 Parameters[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    UINT64 Results[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    shm_bridge_delete_req_t *pReq;
    UINT64 TrustedOsId;

    DebugPrint(DEBUG_INFO, "SmcDestroyShmBridge: uHandle, 0x%x\n", uHandle);
    pReq = (shm_bridge_delete_req_t *)Parameters;
    pReq->uHandle = uHandle;

    TrustedOsId = 0;

    Status = ShmBridgeSysCall(
        SMC_ID_SHM_BRIDGE_DELETE,
        PARAM_ID_SHM_BRIDGE_DELETE,
        Parameters,
        Results,
        &TrustedOsId
        );

    return Status;
}

/**
 * SMC call to TZ to enable shared memory bridge 
 * 
 * 
 * @return STATIC EFI_STATUS 
 */
STATIC EFI_STATUS
SmcEnableShmBridge(
    VOID
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT64 Parameters[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    UINT64 Results[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    UINT64 TrustedOsId;

    TrustedOsId = 0;

    Status = ShmBridgeSysCall(
        SMC_ID_SHM_BRIDGE_ENABLE,
        PARAM_ID_SHM_BRIDGE_ENABLE,
        Parameters,
        Results,
        &TrustedOsId
        );

    return Status;
}

/**
 * Library API for creating a shared memory bridge 
 *  
 * @param pAddr 
 * @param uSize 
 * 
 * @return EFI_STATUS 
 */ 
EFI_STATUS
EFIAPI
CreateShmBridge(
    IN VOID             *pAddr,
    IN UINTN             uSize,
    OUT UINTN           *uHandle
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT64 uShm_bridge_handle = 0;
    ShmMapAddrInfo_t MapInfo = {uSize, (UINTN)pAddr, (UINTN)pAddr};
    ShmDstVMPermInfo_t VMInfo = {AC_VM_HLOS, VM_PERM_R|VM_PERM_W};

    // validate request size
    if (uSize == 0 || (uSize & MEM_ALIGNMENT_MASK)){
        DebugPrint(DEBUG_ERROR, "ShmBridge: CreateShmBridge size invalid, 0x%x\n", uSize);
        return EFI_INVALID_PARAMETER;
    }

    // SMC call to create shared memory bridge
    Status = CreateGenericShmBridge(&MapInfo, &VMInfo, 1, VM_PERM_R|VM_PERM_W, &uShm_bridge_handle);
    DebugPrint(DEBUG_INFO, "ShmBridge: CreateShmBridge handle 0x%x\n", uShm_bridge_handle);
    *uHandle = uShm_bridge_handle;
  
    return Status;
}


/**
 * This API is used to delete shared memory bridge from TZ 
 * 
 * @param uHandle 
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
EFIAPI
DeleteShmBridge(
    IN UINTN            uHandle
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    // delete shared memory bridge from TZ
    Status = SmcDestroyShmBridge(uHandle);
    
    return Status; 
}

/**
 * This API is used to enable shared memory bridge from TZ 
 * 
 * @param
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
EFIAPI
EnableShmBridge(
    VOID
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    // enable shared memory bridge
    Status = SmcEnableShmBridge();
    
    return Status; 
}

/** 
 * Create a generic shared memory bridge with TZ 
 */
EFI_STATUS
EFIAPI
CreateGenericShmBridge(
    IN ShmMapAddrInfo_t   *pMapping,
    IN ShmDstVMPermInfo_t *pDestVMList,
    IN UINT32              uNumDestinations,
    IN UINT32              uPermissions,
    OUT UINTN             *uHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT64 Parameters[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    UINT64 Results[MAX_NUM_SHMBRIDGE_PARAM] = {0};
    shm_bridge_create_req_t *pReq;
    UINT64 uTempPerm = 0;
    UINT64 TrustedOsId;
    BOOLEAN isSelfOwnerPresent = FALSE;
    int i;

    DebugPrint(DEBUG_INFO, "SmcCreateShmBridge: pAddr, %x %x %x\n", pMapping->uInputAddr, pMapping->uOutputAddr, pMapping->uSize);
    pReq = (shm_bridge_create_req_t *)Parameters;
    pReq->uPFNAndNSPermFlags = pMapping->uOutputAddr;      // Bits 63:12
    pReq->uIPFNAndSecurePermFlags = pMapping->uInputAddr; // Bits 63:12
    pReq->uSizeAndFlags = pMapping->uSize;

    if ((pReq->uPFNAndNSPermFlags & MEM_ALIGNMENT_MASK) || 
        (pReq->uSizeAndFlags & MEM_ALIGNMENT_MASK) ||
        (uNumDestinations > MAXSHMVMs)) {
        return EFI_INVALID_PARAMETER;
    }

    pReq->uNSVmids = 0;
    for (i=0; i<uNumDestinations; i++)
    {
        if (pDestVMList[i].uDstVM != AC_VM_HLOS)
        {
            pReq->uNSVmids = pReq->uNSVmids << VM_BITS;
            pReq->uNSVmids |= pDestVMList[i].uDstVM;

            uTempPerm = uTempPerm << PERM_BITS;
            uTempPerm |= pDestVMList[i].uDstVMperm;
        }
        else
        {
            isSelfOwnerPresent = TRUE;
            // set self owner flag
            pReq->uSizeAndFlags |= (1ULL << SELF_OWNER_BIT);
            // set self owner perm
            pReq->uSizeAndFlags |= pDestVMList[i].uDstVMperm << 2;
        }
    }
    if(isSelfOwnerPresent)
    {
        // Reducing the num destinations by 1, because HYP ownership is tracked in self
        // ownership register
        uNumDestinations--;
    }

    pReq->uSizeAndFlags |= uNumDestinations << SHM_NUM_VM_SHIFT;
    pReq->uPFNAndNSPermFlags |= uTempPerm;
    pReq->uIPFNAndSecurePermFlags |= uPermissions;

    DebugPrint(DEBUG_INFO, "SmcCreateShmBridge: pReq, %x %x %x %x\n", pReq->uPFNAndNSPermFlags,
                                                                      pReq->uIPFNAndSecurePermFlags,
                                                                      pReq->uSizeAndFlags,
                                                                      pReq->uNSVmids);
    TrustedOsId = 0;

    Status = ShmBridgeSysCall(
        SMC_ID_SHM_BRIDGE_CREATE,
        PARAM_ID_SHM_BRIDGE_CREATE,
        Parameters,
        Results,
        &TrustedOsId
        );

    if (uHandle != NULL) {
        *uHandle = Results[1];
    }

    return Status;
}
