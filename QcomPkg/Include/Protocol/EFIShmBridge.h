/** 
  file  EFIShmBridge.h
  
  Shared Memory Bridge Protocol for UEFI.

*/
/*=============================================================================
   Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
   Qualcomm Technologies Proprietary and Confidential.

=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/03/18   pranavj Ported SHMBridge changes to Poipu
 06/16/17           Initial version
=============================================================================*/
#ifndef __EFISHMBRIDGE_H__
#define __EFISHMBRIDGE_H__

/*===========================================================================
  INCLUDE FILES
===========================================================================*/
#include <Uefi.h>

/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/
/** @ingroup efi_shmbridge_constants
  Protocol version.
*/
#define EFI_SHMBRIDGE_PROTOCOL_REVISION 0x0000000000010001

/* Protocol GUID definition */
/** @ingroup efi_shmbridge_protocol */
#define EFI_SHMBRIDGE_PROTOCOL_GUID \
   { 0x9c1eb71f, 0xdd6c, 0x4ed5, { 0x9f, 0x6a, 0x5c, 0xc0, 0xca, 0x78, 0x9f, 0x16 } }

/*===========================================================================
  EXTERNAL VARIABLES
===========================================================================*/
extern EFI_GUID gEfiShmBridgeProtocolGuid;

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/
//
// shared memory bridge types issued by application.
// 
typedef enum {
    ShmBridgeDefault,
    ShmBridgeLoaderData,
    ShmBridgeBootSvcData,
    ShmBridgeRunTimeSvcData
} SHMBRIDGE_TYPE;

/** Protocol declaration.  */
typedef struct _EFI_SHMBRIDGE_PROTOCOL EFI_SHMBRIDGE_PROTOCOL;
/** @endcond */

typedef struct _MapAddrInfo
{
    UINT64 uSize; /* In Bytes */
    UINT64 uInputAddr;
    UINT64 uOutputAddr;
} MapAddrInfo;

typedef struct _DstVMPermInfo
{
    UINT32 uDstVM;
    UINT32 uDstVMperm;
} DstVMPermInfo;

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/

/* EFI_SHMBRIDGE_ALLOCATE */ 
/** @ingroup efi_shmbridge_allocate
  @par Summary
  Allocate memory from Shared Memory Bridge.
  
  @return
  EFI_SUCCESS  -- Function completed successfully. \n
  Other values -- Failure.
*/

typedef
VOID *
(EFIAPI *EFI_SHMBRIDGE_ALLOCATE)(
  IN EFI_SHMBRIDGE_PROTOCOL *This,
  IN OUT UINT64             *uSize,
  IN SHMBRIDGE_TYPE         uAppType
);

/* EFI_SHMBRIDGE_FREE */ 
/** @ingroup efi_shmbridge_free
  @par Summary
  Free memory allocated from Shared Memory Bridge.
  
  @return
  EFI_SUCCESS  -- Function completed successfully. \n
  Other values -- Failure.
*/

typedef
EFI_STATUS 
(EFIAPI * EFI_SHMBRIDGE_FREE)
(
  IN EFI_SHMBRIDGE_PROTOCOL *This, 
  IN VOID                   *pAddr
);

/* EFI_SHMBRIDGE_CREATE */ 
/** @ingroup efi_shmbridge_create
  @par Summary
  Create Shared Memory Bridge.
  
  @return
  EFI_SUCCESS  -- Function completed successfully. \n
  Other values -- Failure.
*/

typedef
EFI_STATUS 
(EFIAPI *EFI_SHMBRIDGE_CREATE)(
  IN  EFI_SHMBRIDGE_PROTOCOL *This, 
  IN  MapAddrInfo            *pMapping,
  IN  DstVMPermInfo          *pDestVMList,
  IN  UINT32                  uNumDestinations,
  IN  UINT32                  uPermissions,
  OUT UINTN                  *uHandle
);

/* EFI_SHMBRIDGE_DELETE */ 
/** @ingroup efi_shmbridge_create
  @par Summary
  Create Shared Memory Bridge.
  
  @return
  EFI_SUCCESS  -- Function completed successfully. \n
  Other values -- Failure.
*/

typedef
EFI_STATUS 
(EFIAPI *EFI_SHMBRIDGE_DELETE)(
  IN EFI_SHMBRIDGE_PROTOCOL *This,
  IN UINTN                   uHandle
);

/* EFI_SHMBRIDGE_CREATE */ 
/** @ingroup efi_shmbridge_create
  @par Summary
  Create Shared Memory Bridge.
  
  @return
  EFI_SUCCESS  -- Function completed successfully. \n
  Other values -- Failure.
*/

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_shmbridge_protocol
  @par Summary
  Qualcomm Shared Memory Bridge Protocol interface.

  @par Parameters
  @inputprotoparams{shmbridge_proto_params.tex} 
*/
struct _EFI_SHMBRIDGE_PROTOCOL {
  UINT64                              Revision;
  EFI_SHMBRIDGE_ALLOCATE              ShmBridgeAllocate;
  EFI_SHMBRIDGE_FREE                  ShmBridgeFree;
  EFI_SHMBRIDGE_CREATE                ShmBridgeCreate;
  EFI_SHMBRIDGE_DELETE                ShmBridgeDelete;
};

#endif /* __EFISHMBRIDGE_H__ */


