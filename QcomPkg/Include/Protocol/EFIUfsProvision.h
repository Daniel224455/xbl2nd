/** @file EFIUfsProvision.h
   
  UFS provision protocol definition.

  Copyright (c) 2018 Qualcomm Technologies, Inc. 
  All Rights Reserved. 
  Qualcomm Technologies Proprietary and Confidential

*/

/*=============================================================================
                              EDIT HISTORY


 when        who     what, where, why
 --------    ---     -----------------------------------------------------------
 10/18/18    jt      Initial version
 
=============================================================================*/
#ifndef __EFI_UFS_PROV_H__
#define __EFI_UFS_PROV_H__

/** @cond */
typedef struct _EFI_UFS_PROV_PROTOCOL   EFI_UFS_PROV_PROTOCOL;
/** @endcond */

/** @ingroup efi_ufs_prov_constants
  Protocol version.
*/
#define EFI_UFS_PROV_PROTOCOL_REVISION 0x0000000000010000

/* Protocol GUID definition. */
/** @ingroup efi_ufs_prov_protocol  */
#define EFI_UFS_PROV_PROTOCOL_GUID \
   { 0xD64684D3, 0xE24A, 0x4ACC, { 0xA8, 0x6B, 0x24, 0x1F, 0x2C, 0x42, 0x83, 0x38 } }

/** @cond */
/**
  External reference to the Ufs Prov Protocol GUID 
*/
extern EFI_GUID gEfiUfsProvProtocolGuid;
/** @endcond */

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* EFI_UFS_PROV */
/** @ingroup efi_ufs_prov
  @par Summary
  Provisions the UFS device using the parameters in ConfigFileName. 
 
  @param[in]   This             Pointer to the EFI_UFS_PROV_PROTOCOL instance.
  @param[in]   ConfigFileName   Name of the file containing the provisioning 
                                parameters. 

  @return
  EFI_SUCCESS           -- Function completed successfully.
  EFI_DEVICE_ERROR      -- Failure to provision.
  EFI_INVALID_PARAMETER -- Parameter is invalid.
*/
typedef
EFI_STATUS 
(EFIAPI * EFI_UFS_PROV)(
   IN EFI_UFS_PROV_PROTOCOL *This, 
   IN CONST CHAR8           *ConfigFileName
);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_ufs_prov_protocol
  @par Summary
  UFS provision Protocol interface.

  @par Parameters
  @inputprotoparams{ufs_prov_proto_params.tex} 
*/
struct _EFI_UFS_PROV_PROTOCOL 
{  
   UINT64 Revision;
   EFI_UFS_PROV ProvisionUfs; 
};

#endif	/* __EFI_UFS_PROV_H__ */

