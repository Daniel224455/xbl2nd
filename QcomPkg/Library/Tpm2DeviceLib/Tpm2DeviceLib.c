/** @file
  This library is TPM2 QC TPM device lib.

  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.

  Copyright (c) 2013, Intel Corporation. All rights reserved. <BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


 when       who      what, where, why
 -------   ---      ----------------------------------------------------------
12/20/18   shl      Initial version
=============================================================================*/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Protocol/EFITpm.h>


STATIC EFI_TPM_PROTOCOL  *pTpmProtocol = NULL;

/**
  This service enables the sending of commands to the TPM2.

  @param[in]      InputParameterBlockSize  Size of the TPM2 input parameter block.
  @param[in]      InputParameterBlock      Pointer to the TPM2 input parameter block.
  @param[in,out]  OutputParameterBlockSize Pointer to size of the TPM2 output parameter block.
  @param[in]      OutputParameterBlock     Pointer to the TPM2 output parameter block.

  @retval EFI_SUCCESS            The command byte stream was successfully sent to the device and a response was successfully received.
  @retval EFI_DEVICE_ERROR       The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_BUFFER_TOO_SMALL   The output parameter block is too small. 
**/
EFI_STATUS
EFIAPI
Tpm2SubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if( pTpmProtocol == NULL )
  {
     Status = gBS->LocateProtocol( &gEfiTpmProtocolGuid, NULL, (VOID **)&pTpmProtocol );
     if(EFI_ERROR(Status))
     {
       DEBUG((EFI_D_ERROR, " Locate TPM Protocol failed, Status =  (0x%x)\r\n", Status));
       return Status;
     }
  }

  return pTpmProtocol->SubmitCommand( pTpmProtocol,
                                      InputParameterBlockSize,
                                      InputParameterBlock,
                                      OutputParameterBlockSize,
                                      OutputParameterBlock
                                    );
}

/**
  This service requests use TPM2.

  @retval EFI_SUCCESS   Always return EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
Tpm2RequestUseTpm( VOID )
{
  return EFI_SUCCESS;
}

/**
  This service register TPM2 device.

  @param Tpm2Device  TPM2 device

  @retval EFI_SUCCESS          This TPM2 device is registered successfully.
  @retval EFI_UNSUPPORTED      System does not support register this TPM2 device.
  @retval EFI_ALREADY_STARTED  System already register this TPM2 device.
**/
EFI_STATUS
EFIAPI
Tpm2RegisterTpm2DeviceLib (
  IN TPM2_DEVICE_INTERFACE   *Tpm2Device
  )
{
  return EFI_UNSUPPORTED;
}
