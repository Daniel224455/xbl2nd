/**

  Policy Engine Secure Boot test

  Copyright (c) 2019 Qualcomm Technologies Inc. All rights reserved.

**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/UefiCfgLib.h>

#include <Protocol/VariablePolicy.h>

#include "PK.h"
#include "NullPK.h"
#include "signed_pav.h"
#include "signed_null_pav.h"

#define POLICY_ENGINE_ENABLED       0x7
#define PAV                         L"pav"

#define PAV_GUID \
  {0x77FA9ABD, 0x0359, 0x4D32, {0xBD, 0x60, 0x28, 0xF4, 0xE7, 0x8F, 0x78, 0x4B}}

extern EFI_GUID gEfiGlobalVariableGuid;
extern EFI_GUID gEfiImageSecurityDatabaseGuid;
STATIC EFI_GUID pavGuid = PAV_GUID;

VARIABLE_POLICY_PROTOCOL *vp        = NULL;    /* Variable policy protocol */
UINT8                    SecureBoot = 0;
UINT8                    SetupMode  = 1;


/**
 *
 **/
STATIC
VOID
DoReboot(VOID)
{
  AsciiPrint("Resetting device...\n");
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
}

/**
 *
 **/
STATIC
VOID
PrintSecureBootInfo()
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     Attr   = 0;
  UINT8      Data   = 0;
  UINTN      Size   = sizeof(Data);

  Status = gRT->GetVariable(
              L"SetupMode",
              &gEfiGlobalVariableGuid,
              &Attr,
              &Size,
              &Data);

  AsciiPrint("SetupMode - ");
  if (Status != EFI_SUCCESS) {
    AsciiPrint("[%r]\n", Status);
  } else {
    AsciiPrint("%d\n", Data);
    SetupMode = Data;
  }

  Status = gRT->GetVariable(
              L"SecureBoot",
              &gEfiGlobalVariableGuid,
              &Attr,
              &Size,
              &Data);

  AsciiPrint("SecureBoot - ");
  if (Status != EFI_SUCCESS) {
    AsciiPrint("[%r]\n", Status);
  } else {
    AsciiPrint("%d\n", Data);
    SecureBoot = Data;
  }
}


/**
 *
 **/
STATIC
VOID
ProgramPK()
{

  EFI_STATUS Status                           = EFI_SUCCESS;
  UINT32     Attr                             = (EFI_VARIABLE_NON_VOLATILE |
                                                  EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
  UINTN      Size                             = sizeof(signed_PK);

  AsciiPrint("Programming PK... ");
  Status = gRT->SetVariable(
              L"PK",
              &gEfiGlobalVariableGuid,
              Attr,
              Size,
              signed_PK);
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  Status = FlushVariableServices();
  ASSERT_EFI_ERROR(Status);
}


/**
 *  FAILSAFE
 **/
STATIC
VOID
DeletePKUsingNullSignedKey()
{

  EFI_STATUS Status                           = EFI_SUCCESS;
  UINT32     Attr                             = (EFI_VARIABLE_NON_VOLATILE |
                                                  EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
  UINTN      Size                             = sizeof(signed_NullPK);

  AsciiPrint("Programming NULL PK... ");
  Status = gRT->SetVariable(
              L"PK",
              &gEfiGlobalVariableGuid,
              Attr,
              Size,
              signed_NullPK);
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  Status = FlushVariableServices();
  ASSERT_EFI_ERROR(Status);
}


/**
 *  FAILSAFE
 **/
STATIC
VOID
DeletePAVUsingNullSignedKey()
{

  EFI_STATUS Status                           = EFI_SUCCESS;
  UINT32     Attr                             = (EFI_VARIABLE_NON_VOLATILE |
                                                  EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
  UINTN      Size                             = sizeof(signed_null_pav);

  AsciiPrint("Programming NULL PAV... ");
  Status = gRT->SetVariable(
              PAV,
              &pavGuid,
              Attr,
              Size,
              signed_null_pav);
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  Status = FlushVariableServices();
  ASSERT_EFI_ERROR(Status);
}


/**
 *
 **/
EFI_STATUS
EFIAPI
VariablePolicySecureBootTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
#define MOR_GUID \
  {0xe20939be, 0x32d4, 0x41be, {0xa1, 0x50, 0x89, 0x7f, 0x85, 0xd4, 0x98, 0x29}}

  EFI_GUID   MorGuid                          = MOR_GUID;
  CHAR16*    MorName                          = L"MemoryOverwriteRequestControl";
  EFI_STATUS Status                           = EFI_SUCCESS;
  UINT32     SBAttr                           = EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS;
  UINT32     MORAttr                          = (EFI_VARIABLE_NON_VOLATILE |
                                                  EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS);
  UINT32     AuthAttr                         = (EFI_VARIABLE_NON_VOLATILE |
                                                  EFI_VARIABLE_RUNTIME_ACCESS |
                                                  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);
  UINT32     ConfigEnableVariablePolicyEngine = 0;
  UINT8      Data                             = 0;
  UINTN      Size                             = sizeof(Data);
  BOOLEAN    State                            = FALSE;
  BOOLEAN    NeedReboot                       = FALSE;

  AsciiPrint("Get policy engine protocol... ");
  Status = gBS->LocateProtocol(
      &gVariablePolicyProtocolGuid,
      NULL,
      (VOID **) &vp);
  ASSERT_EFI_ERROR(Status);
  ASSERT(vp != NULL);
  AsciiPrint("PASS\n");

  AsciiPrint("Check policy engine status... ");
  Status = vp->IsVariablePolicyEnabled(&State);
  ASSERT_EFI_ERROR(Status);
  ASSERT(State == TRUE);
  AsciiPrint("PASS\n");

  AsciiPrint("Check policy engine configuration... ");
  Status = GetConfigValue("EnableVariablePolicyEngine", &ConfigEnableVariablePolicyEngine);
  ASSERT_EFI_ERROR(Status);
  ASSERT((ConfigEnableVariablePolicyEngine & POLICY_ENGINE_ENABLED) == POLICY_ENGINE_ENABLED);
  AsciiPrint("PASS\n");

  AsciiPrint("Check if MOR variable exists... ");
  Status = gRT->GetVariable(
             MorName,
             &MorGuid,
             NULL,
             &Size,
             &Data);
  if (Status == EFI_NOT_FOUND)
  {
    AsciiPrint("%r\n", Status);
    AsciiPrint("Adding MOR... ");
    Data = 1;
    Size = sizeof(Data);
    Status = gRT->SetVariable(
               MorName,
               &MorGuid,
               MORAttr,
               Size,
               &Data);
    NeedReboot = TRUE;
  }
  else if (Status == EFI_BUFFER_TOO_SMALL)
    Status = EFI_SUCCESS;  /* buffer too small implies mor exists */

  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  AsciiPrint("Check if private authenticated variable (PAV) exists... ");
  Status = gRT->GetVariable(
             PAV,
             &pavGuid,
             NULL,
             &Size,
             &Data);
  if (Status == EFI_NOT_FOUND)
  {
    AsciiPrint("%r\n", Status);
    AsciiPrint("Adding PAV... ");
    Status = gRT->SetVariable(
               PAV,
               &pavGuid,
               AuthAttr,
               signed_pav_len,
               signed_pav);
    NeedReboot = TRUE;
  }
  else if (Status == EFI_BUFFER_TOO_SMALL)
    Status = EFI_SUCCESS;  /* buffer too small implies PAV exists */

  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  AsciiPrint("Check if PK is programmed...\n");
  PrintSecureBootInfo();
  if (SetupMode == 1) {
    ProgramPK();
    NeedReboot = TRUE;
  }

  if (NeedReboot) {
    AsciiPrint("After device resets, restart the test...\n");
    DoReboot();
  }

  AsciiPrint("Enabling Secure Boot... ");
  Data = 1;
  Size = sizeof(Data);
  Status = gRT->SetVariable(
              L"SecureBoot",
              &gEfiGlobalVariableGuid,
              SBAttr,
              Size,
              &Data);
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  PrintSecureBootInfo();

  AsciiPrint("Attempting to delete PK; expect security violation... ");
  Status = gRT->SetVariable(
              L"PK",
              &gEfiGlobalVariableGuid,
              AuthAttr,
              0,
              NULL);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  AsciiPrint("PASS\n");

  AsciiPrint("Attempting to delete PAV; expect security violation... ");
  Status = gRT->SetVariable(
              PAV,
              &pavGuid,
              AuthAttr,
              0,
              NULL);
  ASSERT(Status == EFI_SECURITY_VIOLATION);
  AsciiPrint("PASS\n");

  AsciiPrint("Disabling Policy Engine... ");
  Status = vp->DisableVariablePolicy();
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  AsciiPrint("Attempting to delete PK again; expect success... ");
  Status = gRT->SetVariable(
              L"PK",
              &gEfiGlobalVariableGuid,
              AuthAttr,
              0,
              NULL);
  if (Status != EFI_SUCCESS) {
    AsciiPrint("FAIL\n");
    DeletePKUsingNullSignedKey();
    DoReboot();
  }
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  AsciiPrint("Attempting to delete PAV again; expect success... ");
  Status = gRT->SetVariable(
              PAV,
              &pavGuid,
              AuthAttr,
              0,
              NULL);
  if (Status != EFI_SUCCESS) {
    AsciiPrint("FAIL\n");
    DeletePAVUsingNullSignedKey();
    DoReboot();
  }
  AsciiPrint("PASS\n");

  AsciiPrint("Attempting to delete MOR; expect success... ");
  Status = gRT->SetVariable(
             MorName,
             &MorGuid,
             MORAttr,
             0,
             NULL);
  ASSERT_EFI_ERROR(Status);
  AsciiPrint("PASS\n");

  FlushVariableServices();

  PrintSecureBootInfo();

  return EFI_SUCCESS;
}

