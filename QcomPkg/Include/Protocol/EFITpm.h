/**
  @file EFITpm.h
  @brief Declaration of Tpm DXE Driver Interface 
*/
/*=============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when        who     what, where, why
 --------    ---    -----------------------------------------------------------
 03/29/19    shl    Removed acpi register API
 12/20/18    shl    Addded some defs 
 07/18/11    shl    Initial version.

=============================================================================*/

#ifndef _EFITPM_H_
#define _EFITPM_H_

#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>

/** @cond */
typedef struct {
 UINT32   Heartbeat;
 UINT32   Error;
 UINT32   Cancel;
 UINT32   Start;
 UINT64   InterruptControl;
 UINT32   CommandSize;
 UINT64   Command;
 UINT32   ResponseSize;
 UINT64   Response;
} __attribute__((packed)) TPM2_ACPI_CONTROL_AREA;


/* define EFI_TPM_PROTOCOL */
typedef struct  _EFI_TPM_PROTOCOL EFI_TPM_PROTOCOL;
/** @endcond */

/*  Tpm UEFI GUID */
/** @cond */
extern EFI_GUID gEfiTpmProtocolGuid;
/** @endcond */

/** @ingroup efi_tpm_constants
  Protocol version. 
*/
#define TPM_REVISION 0x00010004

#define dTPM    0x6454504D  //dTPM
#define fTPM    0x6654504D  //fTPM

/*===========================================================================
  TPM FUNCTION DEFINITIONS
===========================================================================*/
/* EFI_TPM_GET_APPLICATION_ID */
/** @ingroup efi_tpm_get_application_id
  @par Summary
  Gets the Tpm application ID.

  @param[in]  This          Pointer to the EFI_TPM_PROTOCOL instance.
  @param[out] TpmAppIdPtr   Pointer to Tpm app Id.

  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- NULL parameter was passed.

*/
typedef
EFI_STATUS
(EFIAPI *EFI_TPM_GET_APPLICATION_ID)( 
  IN  EFI_TPM_PROTOCOL      *This, 
  OUT UINT32                *TpmAppIdPtr
);


/* EFI_TPM_GET_TYPE */
/** @ingroup efi_tpm_get_type
  @par Summary
  Gets the TPM type, dTPM or fTPM.

  @param[in]  This         Pointer to the EFI_TPM_PROTOCOL instance.
  @param[out] TpmTypePtr   Pointer to TPM type.

  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- NULL parameter was passed.

*/
typedef
EFI_STATUS
(EFIAPI *EFI_TPM_GET_TYPE)( 
  IN  EFI_TPM_PROTOCOL      *This, 
  OUT UINT32                *TpmTypePtr
);


/* EFI_TPM_GET_CONTROL_AREA */
/** @ingroup efi_tpm_get_control_area
  @par Summary
  gets the TPM's control area.

  @param[in]   This                pointer to the EFI_TPM_PROTOCOL instance.
  @param[out]  ControlAreaPtr      pointer to address of the control area.
  @param[out]  ControlAreaSizePtr  pointer to address of the control area size.
  
  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- NULL parameter was passed.

*/
typedef
EFI_STATUS
(EFIAPI *EFI_TPM_GET_CONTROL_AREA)(
  IN  EFI_TPM_PROTOCOL  *This,
  IN  PHYSICAL_ADDRESS  *ControlAreaPtr,
  IN  UINT32            *ControlAreaSizePtr
);


/* EFI_TPM_SUBMIT_COMMAND */
/** @ingroup efi_tpm_submit_command
  @par Summary
  submit tpm command.

  This function submit a command to TZ, this assume caller know the command
  stream and how to read output block. Assume this is calling to fTPM in TZ.

  @param[in]   this                      EFI_TREE_PROTOCOL instance.
  @param[in]   InputParameterBlockSize   Size of input parameter block.
  @param[in]   InputParameterBlock       Pointer to input parameter block.
  @param[out]  OutputParameterBlockSize  Pointer to size of output parameter block.
  @param[out]  OutputParameterBlock      Pointer to output parameter block.

  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- NULL parameter is passed. \n
  EFI_DEVICE_ERROR      -- SCM call failed.

*/
typedef
EFI_STATUS
(EFIAPI *EFI_TPM_SUBMIT_COMMAND)(
  IN  EFI_TPM_PROTOCOL  *This,
  IN UINT32              InputParameterBlockSize,
  IN UINT8              *InputParameterBlock,
  IN UINT32             *OutputParameterBlockSize,
  IN UINT8              *OutputParameterBlock
);

/* EFI_TPM_FLUSH_DISABLE */
/** @ingroup efi_tpm_flush_disable
  @par Summary
  Disables NV flushing.

  @param[in]  This         Pointer to the EFI_TPM_PROTOCOL instance.

  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- NULL parameter was passed.

*/
typedef
EFI_STATUS
(EFIAPI *EFI_TPM_FLUSH_DISABLE)( 
  IN  EFI_TPM_PROTOCOL      *This  
);


/* TPM Protocol; structure is documented with Tpm Protocol above */
/** @cond */
struct _EFI_TPM_PROTOCOL
{ 
  EFI_TPM_GET_APPLICATION_ID     GetTpmAppId;
  EFI_TPM_GET_TYPE               GetTpmType;
  EFI_TPM_GET_CONTROL_AREA       GetControlArea;
  EFI_TPM_SUBMIT_COMMAND         SubmitCommand;
  EFI_TPM_FLUSH_DISABLE          FlushDisable;
};
/** @endcond */

#endif /* _EFITPM_H_ */

