/** @file HSUartDxe.c
   
  HS-UART UEFI Driver code

  Copyright (c) 2018-2019, 2021 Qualcomm Technologies, Inc. All rights reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 01/21/21   PCR     Added changes to support set_baudrate API
 04/22/19   nm      Support de-initialize API
 11/16/18   nm      Initial revision for UEFI HS-UART Driver 

=============================================================================*/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  HS-UART PROTOCOL interfaces
 */
#include <Protocol/EFIHSUart.h>
#include "HSUart.h"
#include "DALSys.h"

/**
  HS-UART UEFI Protocol implementation
 */
EFI_QCOM_HSUART_PROTOCOL HSUartProtocolImplementation = 
{
  HSUART_DXE_REVISION,
  hsuart_initialize,
  hsuart_read,
  hsuart_write,
  hsuart_poll,
  hsuart_deinitialize,
  hsuart_set_baudrate,
};

/**
  HS-UART DXE Driver Entry Point
 */
EFI_STATUS
HSUartProtocolInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS Status;

  DALSYS_InitMod(NULL);

  Status = gBS->InstallMultipleProtocolInterfaces(
	  &ImageHandle,
	  &gQcomHSUartProtocolGuid,
	  &HSUartProtocolImplementation,
	  NULL);

  return Status;
}

