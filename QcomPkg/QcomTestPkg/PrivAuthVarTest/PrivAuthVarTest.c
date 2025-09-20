/** @file PrivAuthVarTest.c
   
  Tests for Private Authenticated Variables.

  Copyright (c) 2018, 2019 Qualcomm Technologies Inc. All rights reserved.
Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/03/19   bd      Ported changes from Napali
 04/04/18   ai      Initial Revision

=============================================================================*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>

#include "unsigned_pav.h"
#include "signed_pav.h"
#include "signed_pav_new.h"
#include "signed_pav_appended.h"
#include "signed_pav_null.h"
#include "signed_pav_null_old.h"
#include "signed_pav_diff_cert.h"

extern EFI_GUID gEfiGlobalVariableGuid;

#define PAV           L"pav"


STATIC
VOID
PrintPAV()
{

  EFI_STATUS Status     = EFI_SUCCESS;
  UINT32 Attr           = 0;
  UINT8 VarData[64]    = {0};
  UINTN VarDataSize     = sizeof(VarData);

  Status = gRT->GetVariable(PAV, &gEfiGlobalVariableGuid,
      &Attr, &VarDataSize, VarData);

  AsciiPrint("Current Value - ", Status);
  if (Status != EFI_SUCCESS) {
    AsciiPrint("[%r]\n", Status);
  } else {
    AsciiPrint("%a\n", VarData);
  }
  AsciiPrint("\n");
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
PrivAuthVarTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status     = EFI_SUCCESS;
  UINT32 Attr           = (EFI_VARIABLE_RUNTIME_ACCESS |
                           EFI_VARIABLE_BOOTSERVICE_ACCESS |
                           EFI_VARIABLE_NON_VOLATILE |
                           EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
  UINT32 Attr_Append    = (EFI_VARIABLE_RUNTIME_ACCESS |
                           EFI_VARIABLE_BOOTSERVICE_ACCESS |
                           EFI_VARIABLE_NON_VOLATILE |
                           EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS |
                           EFI_VARIABLE_APPEND_WRITE);
  UINT8 VarData          = 0;
  UINTN VarDataSize      = sizeof(VarData);

  AsciiPrint("Check if private authenticated variable (PAV) exists...\n");
  Status = gRT->GetVariable(PAV, &gEfiGlobalVariableGuid,
      NULL, &VarDataSize, &VarData);
  AsciiPrint("Return - %r\n", Status);
  ASSERT (Status == EFI_NOT_FOUND);
  PrintPAV();

  AsciiPrint("Adding PAV...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_len, signed_pav);
  AsciiPrint("Return - %r\n", Status);
  ASSERT_EFI_ERROR(Status);
  PrintPAV();

  AsciiPrint("Deleting PAV...\n");
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr & ~EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS,
      0, NULL);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_INVALID_PARAMETER);
  PrintPAV();

  AsciiPrint("Deleting PAV with AUTH flag...\n");
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, 0, NULL);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  PrintPAV();

  AsciiPrint("Adding unsigned PAV...\n");
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, unsigned_pav_len, unsigned_pav);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_INVALID_PARAMETER);
  PrintPAV();

  AsciiPrint("Adding PAV with NEWER timestamp...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav_new);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_new_len, signed_pav_new);
  AsciiPrint("Return - %r\n", Status);
  ASSERT_EFI_ERROR(Status);
  PrintPAV();

  AsciiPrint("Adding PAV back with OLDER timestamp...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_len, signed_pav);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  PrintPAV();

  AsciiPrint("Appending PAV...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav_appended);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr_Append, signed_pav_appended_len, signed_pav_appended);
  AsciiPrint("Return - %r\n", Status);
  ASSERT_EFI_ERROR(Status);
  PrintPAV();

  AsciiPrint("Adding PAV with NEWER timestamp, but different cert...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav_diff_cert);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_diff_cert_len, signed_pav_diff_cert);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  PrintPAV();

  AsciiPrint("Deleting PAV using OLD, NULL signed payload...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav_null_old);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_null_len, signed_pav_null_old);
  AsciiPrint("Return - %r\n", Status);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  PrintPAV();

  AsciiPrint("Deleting PAV using NULL signed payload...\n");
  AsciiPrint("Timestamp - %t\n", signed_pav_null);
  Status = gRT->SetVariable(PAV, &gEfiGlobalVariableGuid,
      Attr, signed_pav_null_len, signed_pav_null);
  AsciiPrint("Return - %r\n", Status);
  ASSERT_EFI_ERROR(Status);
  PrintPAV();

  return EFI_SUCCESS;
}

