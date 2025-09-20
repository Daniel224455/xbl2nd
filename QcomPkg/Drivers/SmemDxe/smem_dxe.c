/**
 * @file smem_dxe.c
 *
 * DXE driver for shared memory in UEFI
 */

/*==============================================================================
     Copyright (c) 2011, 2018 Qualcomm Technologies, Incorporated. 
     All rights reserved.
     Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/15/11   tl      Created UEFI support code for SMEM
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include "Protocol/EFISmem.h"
#include "com_dtypes.h"
#include "Library/smem.h"

/*===========================================================================
                        INTERNAL FUNCTION DEFINITIONS
===========================================================================*/

/**
 * Wrapper function for smem_alloc(): Allocates a shared memory item.
 *
 * See the documentation for smem_alloc() for the detailed semantics of this
 * function.
 *
 * @param[in]   item    The shared memory item to allocate.
 * @param[in]   size    The size to allocate.
 * @param[out]  addr    The address in shared memory of the requested item.
 *
 * @return EFI_SUCCESS
 */
static EFI_STATUS EFI_smem_alloc
(
  smem_mem_type item,
  UINT32        size,
  void        **addr
)
{
  *addr = smem_alloc(item, size);
  return EFI_SUCCESS;
}

/**
 * Wrapper function for smem_get_addr(): Returns the address of an item if it
 * has been allocated in shared memory.
 *
 * See the documentation for smem_get_addr() for the detailed semantics of
 * this function.
 *
 * @param[in]   item    The shared memory item.
 * @param[out]  size    The size of the allocated item.
 * @param[out]  addr    The address in shared memory of the requested item, if
 *                      the item has already been allocated.
 *
 * @return EFI_SUCCESS
 */
static EFI_STATUS EFI_smem_get_addr
(
  smem_mem_type item,
  UINT32       *size,
  void        **addr
)
{
  uint32 s;
  *addr = smem_get_addr(item, &s);
  *size = s;
  return EFI_SUCCESS;
}

/**
 * Translates the SMEM error code to EFI error code
 *
 * @param[in]   smem_err    SMEM error code.
 *
 * @return EFI error code
 */
static EFI_STATUS smem_err_to_efi_err(int32 smem_err)
{
  switch(smem_err)
  {
    case SMEM_STATUS_SUCCESS:
      return EFI_SUCCESS;
      
    case SMEM_STATUS_FAILURE:
      return EFI_UNSUPPORTED;
      
    case SMEM_STATUS_INVALID_PARAM:
      return EFI_INVALID_PARAMETER;
      
    case SMEM_STATUS_OUT_OF_RESOURCES:
      return EFI_OUT_OF_RESOURCES;
      
    case SMEM_STATUS_NOT_FOUND:
      return EFI_NOT_FOUND;
      
    case SMEM_STATUS_DUPLICATE:
      return EFI_ALREADY_STARTED;
      
    case SMEM_STATUS_SIZE_MISMATCH:
      return EFI_BAD_BUFFER_SIZE;
      
    default:
      return EFI_UNSUPPORTED;
  }
}

/**
 * Wrapper function for smem_alloc_ex(): Allocates a shared memory item.
 *
 * See the documentation for smem_alloc_ex() for the detailed semantics of this
 * function.
 *
 * @param[inout] params  See definition of smem_alloc_params_type for details.
 *
 * @return EFI_SUCCESS on success otherwise EFI_XXXX error code.
 */
static EFI_STATUS EFI_smem_alloc_ex
(
  smem_alloc_params_type *params
)
{
  int32 ret;
  
  ret = smem_alloc_ex(params);
  
  return smem_err_to_efi_err(ret);
}

/**
 * Wrapper function for smem_get_addr_ex(): Returns the address of an item if it
 * has been allocated in shared memory.
 *
 * See the documentation for smem_get_addr_ex() for the detailed semantics of
 * this function.
 *
 * @param[inout] params  See definition of smem_alloc_params_type for details.
 *
 * @return EFI_SUCCESS on success otherwise EFI_XXXX error code.
 */
static EFI_STATUS EFI_smem_get_addr_ex
(
  smem_alloc_params_type *params
)
{
  int32 ret;
  
  ret = smem_get_addr_ex(params);
  
  return smem_err_to_efi_err(ret);
}

/*===========================================================================
                           INTERNAL DATA STRUCTURES
===========================================================================*/

static EFI_SMEM_PROTOCOL SMEMProtocol =
{
  EFI_SMEM_PROTOCOL_REVISION,
  EFI_smem_alloc,
  EFI_smem_get_addr,
  EFI_smem_alloc_ex,
  EFI_smem_get_addr_ex
};

/*===========================================================================
                        EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/

/**
 * Entry point for the SMEM DXE driver.
 */
EFI_STATUS EFIAPI SmemEntryPoint
(
  EFI_HANDLE            ImageHandle,
  EFI_SYSTEM_TABLE     *SystemTable
)
{
  EFI_HANDLE handle = NULL;
  EFI_STATUS status;

  smem_init();

  status = gBS->InstallMultipleProtocolInterfaces(&handle,
      &gEfiSMEMProtocolGuid, (void **)&SMEMProtocol, NULL, NULL, NULL );

  return status;
}
