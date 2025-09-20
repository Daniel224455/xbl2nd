/**

 Copyright (c) 2017-2018 Qualcomm Technologies, Inc. All Rights
 Reserved.
 

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/03/18   pranavj Ported SHMBridge changes to Poipu
 06/15/17           Initial version

=============================================================================*/
#ifndef __SHMBRIDGE_LIB__H__
#define __SHMBRIDGE_LIB__H__

#include <Uefi.h>

#define MEM_ALIGNMENT_SIZE 0x1000
#define MEM_ALIGNMENT_MASK 0xFFF

#define SMC_ID_SHM_BRIDGE_ENABLE 0x02000C1C
#define SMC_ID_SHM_BRIDGE_DELETE 0x02000C1D
#define SMC_ID_SHM_BRIDGE_CREATE 0x02000C1E

#define PARAM_ID_SHM_BRIDGE_ENABLE 0
#define PARAM_ID_SHM_BRIDGE_DELETE 1
#define PARAM_ID_SHM_BRIDGE_CREATE 4

typedef struct SHM_BRIDGE_CREATE_REQ {
    UINT64 uPFNAndNSPermFlags;
    UINT64 uIPFNAndSecurePermFlags;
    UINT64 uSizeAndFlags;
    UINT64 uNSVmids; 
} shm_bridge_create_req_t;

typedef struct SHM_BRIDGE_DELETE_REQ {
    UINT64 uHandle; 
} shm_bridge_delete_req_t;

typedef struct _ShmMapAddrInfo
{
    UINT64 uSize; /* In Bytes */
    UINT64 uInputAddr;
    UINT64 uOutputAddr;
} ShmMapAddrInfo_t;

typedef struct _ShmDstVMPermInfo
{
    UINT32 uDstVM;
    UINT32 uDstVMperm;
} ShmDstVMPermInfo_t;

/** 
 * Create a shared memory bridge with TZ 
 * 
 * @param pAddr shared memory bridge address pointer
 * @param uAppType appication type 
 * @param uHanlde shared memory handle  
 *  
 * @retval EFI_SUCCESS 
 * @retval EFI_INVALID_PARAMETER 
 * @retval EFI_OUT_OF_RESOURCES 
 * @retVal RETURN_DEVICE_ERROR 
 */ 
EFI_STATUS
EFIAPI
CreateShmBridge( 
    IN VOID            *pAddr,
    IN UINTN            uSize,
    OUT UINTN          *uHanlde
);

/** 
 * Delete a shared memory bridge with TZ 
 *  
 * @param uHandle shared memory bridge handle
 *  
 * @retval EFI_SUCCESS 
 * @retval EFI_INVALID_PARAMETER 
 * @retval RETURN_DEVICE_ERROR 
 *  
 */ 
EFI_STATUS
EFIAPI
DeleteShmBridge( 
    IN UINTN            uHanlde
); 

/** 
 * Enable a shared memory bridge with TZ 
 *  
 *  
 * @retval EFI_SUCCESS 
 * @retval RETURN_DEVICE_ERROR 
 *  
 */ 
EFI_STATUS
EFIAPI
EnableShmBridge( 
    VOID
); 

/**
 * create a generic shared memory bridge
 */
EFI_STATUS
EFIAPI
CreateGenericShmBridge(
    IN ShmMapAddrInfo_t   *pMapping,
    IN ShmDstVMPermInfo_t *pDestVMList,
    IN UINT32              uNumDestinations,
    IN UINT32              uPermissions,
    OUT UINTN             *uHandle
);
#endif /* __SHMBRIDGE_LIB__H__ */

