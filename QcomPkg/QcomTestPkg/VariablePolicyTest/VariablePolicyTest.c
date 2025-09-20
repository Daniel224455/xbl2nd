/**

  Variable Policy protocol test

  Copyright (c) 2019,2020 Qualcomm Technologies Inc. All rights reserved.

**/

#include <Uefi.h>
#include <Protocol/VariablePolicy.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Guid/GlobalVariable.h>

#ifndef ARRAY_SIZE
#define  ARRAY_SIZE(a) (sizeof((a))/sizeof((a)[0]))
#endif

#define MAX_WCHARS                  64
#define PAGES                       16

VARIABLE_POLICY_PROTOCOL *vp          = NULL;    /* Variable policy protocol */
UINTN                    TotalSz      = 0;       /* Total size of all policies added by the tests */
UINT8*                   DumpPolicies = NULL;    /* Buffer passed to S-world to dump all policies */
UINT32                   DumpPolicySz = 0;       /* Size of the buffer passed to S-world to dump all policies */
UINT8*                   AllPolicies  = NULL;    /* All policies added by the test concatenated together */
UINT8*                   AllPolicyPtr = NULL;    /* Pointer into AllPolicies buffer */

CHAR16                   VarName[MAX_WCHARS]           = {0};
CHAR16                   LockPolicyVarName[MAX_WCHARS] = {0};

VARIABLE_POLICY_ENTRY PolicyTemplate = {
  .Version            = VARIABLE_POLICY_ENTRY_REVISION,
  .Size               = sizeof(VARIABLE_POLICY_ENTRY),
  .OffsetToName       = sizeof(VARIABLE_POLICY_ENTRY),
  .Namespace          = EFI_GLOBAL_VARIABLE,
  .MinSize            = 0,
  .MaxSize            = MAX_UINT32,
  .AttributesMustHave = 0,
  .AttributesCantHave = 0,
  .LockPolicyType     = NoLock
};

VARIABLE_LOCK_ON_VAR_STATE_POLICY LockPolicyTemplate = {
  .Namespace          = EFI_GLOBAL_VARIABLE,
  .Value              = 1,
};


/**
 *
 */
EFI_STATUS
VarPolTestDumpNullPolicies()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINT32     Size   = 0;

  TEST_START("VarPolTest-DumpNullPolicies");

  /* Dump policies - none installed - Size = 0*/
  Size = 0;
  Status = vp->DumpVariablePolicy(NULL, &Size);
  ASSERT((Status == EFI_SUCCESS) || (Status == EFI_BUFFER_TOO_SMALL));

  /* Dump policies - none installed - Size > 0 */
  Size = DumpPolicySz;
  Status = vp->DumpVariablePolicy(DumpPolicies, &Size);
  ASSERT_EFI_ERROR(Status);

  if (Size > 0)
  {
    TotalSz += Size;
    CopyMem(AllPolicyPtr, DumpPolicies, Size);
  }

  TestStatus("VarPolTest-DumpNullPolicies", Status);
  TEST_STOP("VarPolTest-DumpNullPolicies");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestSanity()
{
  EFI_STATUS                        Status           = EFI_INVALID_PARAMETER;
  VARIABLE_LOCK_ON_VAR_STATE_POLICY *LockPolicyPtr   = NULL;
  UINTN                             BuffSz           = 0;
  UINTN                             Len              = 0;
CHAR16                              *PolicyEntryName = NULL;

  TEST_START("VarPolTest-Sanity");

  /* Register NULL policy */
  Status = vp->RegisterVariablePolicy(NULL);
  ASSERT(Status == EFI_INVALID_PARAMETER);


  /*setup*/
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockFail", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolFail = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolFail != NULL);

  CopyMem(PolFail, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolFail + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolFail->LockPolicyType = NoLock;
  PolFail->Size = BuffSz;


  /* Fail - bad size  - too small */
  PolFail->Size = sizeof(VARIABLE_POLICY_ENTRY) - 1;     /* bad size */

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->Size = BuffSz;


  /* Fail - bad offsettoname - too small */
  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) - 1;   /* bad offset */
  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY);


  /* Fail - bad offsettoname - too large */
  PolFail->OffsetToName = BuffSz + 1;                 /* bad offset */
  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY);


  /* Fail - bad offsettoname - junk space */
#define JUNK_SPACE2  2
  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + JUNK_SPACE2;

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY);


  /* Fail - bad offsettoname - unaligned */
#define JUNK_SPACE1  1
  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + JUNK_SPACE1;

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY);


  /* Fail - bad max/min size */
  PolFail->MinSize = 100;
  PolFail->MaxSize = PolFail->MinSize - 1;   /* bad size */

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->MinSize = 0;
  PolFail->MaxSize = MAX_UINT32;


  /* Fail - bad max/min size - zero */
  PolFail->MinSize = 0;   /* bad size */
  PolFail->MaxSize = PolFail->MinSize;   /* bad size */

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->MaxSize = MAX_UINT32;


  /* Fail - bad lockpolicytype */
  PolFail->LockPolicyType = LockOnVarState + 1;

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail->LockPolicyType = NoLock;


  /* Fail - name not null terminated */
  PolicyEntryName = (UINT16*)((UINT8*)PolFail + PolFail->OffsetToName);
  Len = StrLen(VarName);
  PolicyEntryName[Len] = L'A';    /* overwrite null */

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolicyEntryName[Len] = L'\0';


  /* Fail - null char in Policy entry name */
  PolicyEntryName[0] = L'\0';

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolicyEntryName[0] = L'L';


  /* Fail - name contains # and digits */
  PolicyEntryName[Len - 2] = L'#';
  PolicyEntryName[Len - 1] = L'0';

  Status = vp->RegisterVariablePolicy(PolFail);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolicyEntryName[Len - 2] = L'i';
  PolicyEntryName[Len - 1] = L'l';


  /*setup*/
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockFail2", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);
  Status = StrnCpyS(LockPolicyVarName, MAX_WCHARS, L"LockSignal", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolFail2 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolFail2 != NULL);

  CopyMem(PolFail2, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolFail2 + sizeof(VARIABLE_POLICY_ENTRY),
      &LockPolicyTemplate,
      sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY));
  CopyMem((UINT8*)PolFail2 + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY),
      LockPolicyVarName,
      StrSize(LockPolicyVarName));
  CopyMem((UINT8*)PolFail2 + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName),
      VarName,
      StrSize(VarName));
  PolFail2->Size = BuffSz;
  PolFail2->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + \
    sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName);
  PolFail2->LockPolicyType = LockOnVarState;


  /* Fail - LockPolicy[] - bad size */
  PolFail2->Size = sizeof(VARIABLE_POLICY_ENTRY) + 1;

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail2->Size = BuffSz;


  /* Fail - LockPolicy[] - bad offsettoname */
  PolFail2->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + 1;

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail2->Size = BuffSz;


  /* Fail - LockPolicy[] - lock policy name size not aligned */
  PolFail2->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + \
    sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName) - 1;

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  PolFail2->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + \
    sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName);


  /* Fail - LockPolicy[] - lock policy name not null terminated */
  LockPolicyPtr = (VARIABLE_LOCK_ON_VAR_STATE_POLICY *)((UINT8*)PolFail2 + sizeof(VARIABLE_POLICY_ENTRY));
  Len = StrLen(LockPolicyPtr->Name);
  LockPolicyPtr->Name[Len] = L'A';

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  LockPolicyPtr->Name[Len] = L'\0';


  /* Fail - LockPolicy[] - lockpolicy name is null */
  LockPolicyPtr->Name[0] = L'\0';

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  LockPolicyPtr->Name[0] = L'L';


  /* Fail - LockPolicy[] - lockpolicy name contains wildcard */
  LockPolicyPtr->Name[Len - 2] = L'#';

  Status = vp->RegisterVariablePolicy(PolFail2);
  ASSERT(Status == EFI_INVALID_PARAMETER);

  LockPolicyPtr->Name[Len - 2] = L'l';


  /*setup*/
  BuffSz = MAX_UINT16;
  VARIABLE_POLICY_ENTRY *PolFail3 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolFail3 != NULL);

  CopyMem(PolFail3, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  SetMem((UINT8*)PolFail3 + sizeof(VARIABLE_POLICY_ENTRY), L'A',
      BuffSz - sizeof(VARIABLE_POLICY_ENTRY));
  PolFail3->LockPolicyType = NoLock;
  PolFail3->Size = BuffSz;


  /* Fail - policy too big - expect out of resources */
  Status = vp->RegisterVariablePolicy(PolFail3);
  ASSERT(Status == EFI_OUT_OF_RESOURCES);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-Sanity", Status);
  TEST_STOP("VarPolTest-Sanity");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestNoLock()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  UINT32     Data32 = 0;

  TEST_START("VarPolTest-NoLock");

  /* NoLock */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"NoLock", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolNoLock = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolNoLock != NULL);

  CopyMem(PolNoLock, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolNoLock + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolNoLock->Size = BuffSz;
  PolNoLock->LockPolicyType = NoLock;

  /* Test constraints */
  PolNoLock->MinSize = sizeof(UINT8);
  PolNoLock->MaxSize = sizeof(UINT8);
  PolNoLock->AttributesMustHave = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  PolNoLock->AttributesCantHave = EFI_VARIABLE_RUNTIME_ACCESS;

  Status = vp->RegisterVariablePolicy(PolNoLock);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolNoLock, BuffSz);
  AllPolicyPtr += BuffSz;

  /* Add variable - bad attributes */
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Add variable */
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Delete variable */
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT_EFI_ERROR(Status);


  /* Add variable again */
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - size too large - expect write protected */
  Status = gRT->SetVariable(
              L"NoLock",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data32),
              &Data32);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Register NoLock again - expect already started */
  Status = vp->RegisterVariablePolicy(PolNoLock);
  ASSERT(Status == EFI_ALREADY_STARTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-NoLock", Status);
  TEST_STOP("VarPolTest-NoLock");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLockOnCreate()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;

  TEST_START("VarPolTest-LockOnCreate");

  /* Lock on Create */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockOnCreate", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockOnCreate = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockOnCreate != NULL);

  CopyMem(PolLockOnCreate, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockOnCreate + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolLockOnCreate->Size = BuffSz;
  PolLockOnCreate->LockPolicyType = LockOnCreate;

  Status = vp->RegisterVariablePolicy(PolLockOnCreate);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolLockOnCreate, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable */
  Status = gRT->SetVariable(
              L"LockOnCreate",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect write protected */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"LockOnCreate",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockOnCreate",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-LockOnCreate", Status);
  TEST_STOP("VarPolTest-LockOnCreate");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLockOnCreate2()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;

  TEST_START("VarPolTest-LockOnCreate2");

  /* Add variable before policy */
  Status = gRT->SetVariable(
              L"LockOnCreate2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);

  /* Lock on Create */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockOnCreate2", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockOnCreate2 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockOnCreate2 != NULL);

  CopyMem(PolLockOnCreate2, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockOnCreate2 + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolLockOnCreate2->Size = BuffSz;
  PolLockOnCreate2->LockPolicyType = LockOnCreate;

  Status = vp->RegisterVariablePolicy(PolLockOnCreate2);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolLockOnCreate2, BuffSz);
  AllPolicyPtr += BuffSz;

  /* Add variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockOnCreate2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Modify variable - expect write protected */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"LockOnCreate2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockOnCreate2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-LockOnCreate2", Status);
  TEST_STOP("VarPolTest-LockOnCreate2");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLockNow()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;

  TEST_START("VarPolTest-LockNow");

  /* Lock Now -- prevents variable creation */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockNow", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockNow = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockNow != NULL);

  CopyMem(PolLockNow, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockNow + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolLockNow->Size = BuffSz;
  PolLockNow->LockPolicyType = LockNow;

  Status = vp->RegisterVariablePolicy(PolLockNow);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolLockNow, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockNow",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-LockNow", Status);
  TEST_STOP("VarPolTest-LockNow");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLockNow2()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;

  TEST_START("VarPolTest-LockNow2");

  /* Add variable - should succeed */
  Status = gRT->SetVariable(
              L"LockNow2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);

  /* Lock Now2 -- prevents variable update */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockNow2", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockNow2 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockNow2 != NULL);

  CopyMem(PolLockNow2, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockNow2 + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolLockNow2->Size = BuffSz;
  PolLockNow2->LockPolicyType = LockNow;

  Status = vp->RegisterVariablePolicy(PolLockNow2);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolLockNow2, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Modify variable - expect write protected */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"LockNow2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockNow2",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);

  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-LockNow2", Status);
  TEST_STOP("VarPolTest-LockNow2");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLockOnVarState()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;

  TEST_START("VarPolTest-LockOnVarState");

  /* Lock on Var State */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockOnVarState", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);
  Status = StrnCpyS(LockPolicyVarName, MAX_WCHARS, L"LockSignal", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockOnVarState = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockOnVarState != NULL);

  CopyMem(PolLockOnVarState, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY),
      &LockPolicyTemplate,
      sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY));
  CopyMem((UINT8*)PolLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY),
      LockPolicyVarName,
      StrSize(LockPolicyVarName));
  CopyMem((UINT8*)PolLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName),
      VarName,
      StrSize(VarName));
  PolLockOnVarState->Size = BuffSz;
  PolLockOnVarState->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + \
    sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName);
  PolLockOnVarState->LockPolicyType = LockOnVarState;

  Status = vp->RegisterVariablePolicy(PolLockOnVarState);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolLockOnVarState, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable */
  Status = gRT->SetVariable(
              L"LockOnVarState",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Add lock signal */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"LockSignal",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect write protected*/
  Status = gRT->SetVariable(
              L"LockOnVarState",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected*/
  Status = gRT->SetVariable(
              L"LockOnVarState",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);

  /* Change lock signal */
  Data8 = 0;
  Status = gRT->SetVariable(
              L"LockSignal",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect pass */
  Status = gRT->SetVariable(
              L"LockOnVarState",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-LockOnVarState", Status);
  TEST_STOP("VarPolTest-LockOnVarState");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestDumpPolicies()
{
  EFI_STATUS Status     = EFI_INVALID_PARAMETER;
  UINT32     Size       = 0;
  INTN       CompareVal = 0;

  TEST_START("VarPolTest-DumpPolicies");

  /* DumpVariablePolicy with NULL Buffer */
  Size = 0;
  Status = vp->DumpVariablePolicy(NULL, &Size);
  ASSERT(Status == EFI_BUFFER_TOO_SMALL);
  ASSERT(Size == TotalSz);


  /* DumpVariablePolicy */
  Size = DumpPolicySz;
  Status = vp->DumpVariablePolicy(DumpPolicies, &Size);
  ASSERT_EFI_ERROR(Status);
  ASSERT(Size == TotalSz);

  CompareVal = 1;
//TODO - Variable policies are returned in variable enumeration order...
//       which can be different from the order in which polices were added.
//       If any policies were added before starting the test, then we
//       cannot compare the policies returned with the policies added by this
//       test, even if query the policies orderd before the test. This needs to
//       be rewritten by creating an array of policies added by the test
//       and then checking each policy in the array against the buffer returned.
//       NOTE: we do not add API to delete all policies as that can create a
//       security hole.
#if 0
  CompareVal = CompareMem(AllPolicies, DumpPolicies, TotalSz);
  ASSERT(CompareVal == 0); /* kw */
#endif

  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-DumpPolicies", Status);
  TEST_STOP("VarPolTest-DumpPolicies");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestHashWildcardLockNow()
{
#define WC_GUID \
  { \
    0xc623238e, 0xe62f, 0x4ee0, {0xb8, 0x52, 0x50, 0xa7, 0x4b, 0x35, 0x4e, 0x52 } \
  }
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  EFI_GUID   WCGuid   = WC_GUID;

  TEST_START("VarPolTest-HashWildCard");

  /* Wildcard - # */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"Lock####", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolWildcardPound = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardPound != NULL);

  CopyMem(PolWildcardPound, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolWildcardPound + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolWildcardPound->Size = BuffSz;
  PolWildcardPound->LockPolicyType = LockNow;
  CopyMem(&PolWildcardPound->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardPound);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolWildcardPound, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable - expect write protected */
  Status = gRT->SetVariable(
              L"Lock0000",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Add non-numeric variable - expect write protected */
  Status = gRT->SetVariable(
              L"LockABCD",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Wildcard - # and numbers - expect invalid parameter */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"Lock00##", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolWildcardPound2 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardPound2 != NULL);

  CopyMem(PolWildcardPound2, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolWildcardPound2 + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolWildcardPound2->Size = BuffSz;
  PolWildcardPound2->LockPolicyType = LockNow;
  CopyMem(&PolWildcardPound2->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardPound2);
  ASSERT(Status == EFI_INVALID_PARAMETER);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-HashWildCard", Status);
  TEST_STOP("VarPolTest-HashWildCard");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestNamespaceWildcardLockNow()
{
#define WC_GUID2 \
  { \
    0xc623238f, 0xe62f, 0x4ee0, {0xb8, 0x52, 0x50, 0xa7, 0x4b, 0x35, 0x4e, 0x52 } \
  }

  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  EFI_GUID   WCGuid   = WC_GUID2;

  TEST_START("VarPolTest-NamespaceWildCard");

  /* Add variable */
  Status = gRT->SetVariable(
              L"LockWXYZ",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);

  /* Wildcard - null name */
  BuffSz = sizeof(VARIABLE_POLICY_ENTRY);
  VARIABLE_POLICY_ENTRY *PolWildcardNamespace = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardNamespace != NULL);

  CopyMem(PolWildcardNamespace, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  PolWildcardNamespace->Size = BuffSz;
  PolWildcardNamespace->LockPolicyType = LockNow;
  CopyMem(&PolWildcardNamespace->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardNamespace);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolWildcardNamespace, BuffSz);
  AllPolicyPtr += BuffSz;

  /* Any variable action should fail */
  Status = gRT->SetVariable(
              L"LockABCD",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);

  Status = gRT->SetVariable(
              L"LockWXYZ",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Wildcard again - expect fail */
  Status = vp->RegisterVariablePolicy(PolWildcardNamespace);
  ASSERT(Status == EFI_ALREADY_STARTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-NamespaceWildCard", Status);
  TEST_STOP("VarPolTest-NamespaceWildCard");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestNamespaceWildcardLockOnCreate()
{
#define WC_GUID3 \
  { \
    0xc6232390, 0xe62f, 0x4ee0, {0xb8, 0x52, 0x50, 0xa7, 0x4b, 0x35, 0x4e, 0x52 } \
  }

  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  EFI_GUID   WCGuid   = WC_GUID3;

  TEST_START("VarPolTest-WildcardLockOnCreate");

  /* Lock on Create */
  BuffSz = sizeof(VARIABLE_POLICY_ENTRY);
  VARIABLE_POLICY_ENTRY *PolWildcardLockOnCreate = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardLockOnCreate != NULL);

  CopyMem(PolWildcardLockOnCreate, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  PolWildcardLockOnCreate->LockPolicyType = LockOnCreate;
  CopyMem(&PolWildcardLockOnCreate->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardLockOnCreate);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolWildcardLockOnCreate, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable */
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect write protected */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected */
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-WildcardLockOnCreate", Status);
  TEST_STOP("VarPolTest-WildcardLockOnCreate");
  return Status;
}

/**
 *
 */
EFI_STATUS
VarPolTestNamespaceWildcardLockOnCreate2()
{
#define WC_GUID4 \
  { \
    0xc6232391, 0xe62f, 0x4ee0, {0xb8, 0x52, 0x50, 0xa7, 0x4b, 0x35, 0x4e, 0x52 } \
  }

  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  EFI_GUID   WCGuid   = WC_GUID4;

  TEST_START("VarPolTest-WildcardLockOnCreate2");

  /* Add variable - expect success */
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate2",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Lock on Create */
  BuffSz = sizeof(VARIABLE_POLICY_ENTRY);
  VARIABLE_POLICY_ENTRY *PolWildcardLockOnCreate2 = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardLockOnCreate2 != NULL);

  CopyMem(PolWildcardLockOnCreate2, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  PolWildcardLockOnCreate2->LockPolicyType = LockOnCreate;
  CopyMem(&PolWildcardLockOnCreate2->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardLockOnCreate2);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolWildcardLockOnCreate2, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Modify variable - expect write protected */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate2",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected */
  Status = gRT->SetVariable(
              L"WildcardLockOnCreate2",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-WildcardLockOnCreate2", Status);
  TEST_STOP("VarPolTest-WildcardLockOnCreate2");
  return Status;
}

/**
 *
 */
EFI_STATUS
VarPolTestNamespaceWildcardLockOnVarState()
{
#define WC_GUID5 \
  { \
    0xc6232392, 0xe62f, 0x4ee0, {0xb8, 0x52, 0x50, 0xa7, 0x4b, 0x35, 0x4e, 0x52 } \
  }

  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  UINT8      Data8  = 0;
  EFI_GUID   WCGuid   = WC_GUID5;

  TEST_START("VarPolTest-WildcardLockOnVarState");

  /* Lock on Var State */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"WildcardLockOnVarState", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);
  Status = StrnCpyS(LockPolicyVarName, MAX_WCHARS, L"WildcardLockSignal", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolWildcardLockOnVarState = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolWildcardLockOnVarState != NULL);

  CopyMem(PolWildcardLockOnVarState, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolWildcardLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY),
      &LockPolicyTemplate,
      sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY));
  CopyMem((UINT8*)PolWildcardLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY),
      &WCGuid,
      sizeof(EFI_GUID));
  CopyMem((UINT8*)PolWildcardLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY),
      LockPolicyVarName,
      StrSize(LockPolicyVarName));
  CopyMem((UINT8*)PolWildcardLockOnVarState + sizeof(VARIABLE_POLICY_ENTRY) + \
        sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName),
      VarName,
      StrSize(VarName));
  PolWildcardLockOnVarState->Size = BuffSz;
  PolWildcardLockOnVarState->OffsetToName = sizeof(VARIABLE_POLICY_ENTRY) + \
    sizeof(VARIABLE_LOCK_ON_VAR_STATE_POLICY) + StrSize(LockPolicyVarName);
  PolWildcardLockOnVarState->LockPolicyType = LockOnVarState;
  CopyMem(&PolWildcardLockOnVarState->Namespace, &WCGuid, sizeof(EFI_GUID));

  Status = vp->RegisterVariablePolicy(PolWildcardLockOnVarState);
  ASSERT_EFI_ERROR(Status);

  TotalSz += BuffSz;
  CopyMem(AllPolicyPtr, PolWildcardLockOnVarState, BuffSz);
  AllPolicyPtr += BuffSz;


  /* Add variable */
  Status = gRT->SetVariable(
              L"WildcardLockOnVarState",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Add lock signal */
  Data8 = 1;
  Status = gRT->SetVariable(
              L"WildcardLockSignal",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect write protected*/
  Status = gRT->SetVariable(
              L"WildcardLockOnVarState",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Delete variable - expect write protected*/
  Status = gRT->SetVariable(
              L"WildcardLockOnVarState",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              0,
              NULL);
  ASSERT(Status == EFI_WRITE_PROTECTED);

  /* Change lock signal */
  Data8 = 0;
  Status = gRT->SetVariable(
              L"WildcardLockSignal",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  /* Modify variable - expect pass */
  Status = gRT->SetVariable(
              L"WildcardLockOnVarState",
              &WCGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(Data8),
              &Data8);
  ASSERT_EFI_ERROR(Status);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-WildcardLockOnVarState", Status);
  TEST_STOP("VarPolTest-WildcardLockOnVarState");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestLock()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BuffSz = 0;
  BOOLEAN    State  = FALSE;

  TEST_START("VarPolTest-Lock");

  /* LockVariablePolicy */
  Status = vp->LockVariablePolicy();
  ASSERT_EFI_ERROR(Status);


  /* Register policy after locking */
  Status = StrnCpyS(VarName, MAX_WCHARS, L"LockFail", MAX_WCHARS-1);
  ASSERT_EFI_ERROR(Status);

  BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
  VARIABLE_POLICY_ENTRY *PolLockFail = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
  ASSERT(PolLockFail != NULL);

  CopyMem(PolLockFail, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
  CopyMem((UINT8*)PolLockFail + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
  PolLockFail->Size = BuffSz;
  PolLockFail->LockPolicyType = LockNow;

  Status = vp->RegisterVariablePolicy(PolLockFail);
  ASSERT(Status == EFI_WRITE_PROTECTED);


  /* Disable after lock - expect write protect */
  Status = vp->DisableVariablePolicy();
  ASSERT(Status = EFI_WRITE_PROTECTED);


  /* PE should still be enabled after disable attempt */
  Status = vp->IsVariablePolicyEnabled(&State);
  ASSERT_EFI_ERROR(Status);
  ASSERT(State == TRUE);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-Lock", Status);
  TEST_STOP("VarPolTest-Lock");
  return Status;
}


/**
 *
 */
EFI_STATUS
VarPolTestInit()
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  BOOLEAN    State  = FALSE;

  TEST_START("VarPolTest-Init");

  Status = gBS->LocateProtocol(
      &gVariablePolicyProtocolGuid,
      NULL,
      (VOID **) &vp);
  ASSERT_EFI_ERROR(Status);
  ASSERT(vp != NULL);

  Status = vp->IsVariablePolicyEnabled(&State);
  ASSERT_EFI_ERROR(Status);
  ASSERT(State == TRUE);


  AllPolicies = AllocatePages(PAGES);    /* Policies created in NS world */
  ASSERT (AllPolicies != NULL);
  AllPolicyPtr = AllPolicies;

  DumpPolicies = AllocatePages(PAGES);   /* Policies returned by S-world */
  ASSERT(DumpPolicies != NULL);
  DumpPolicySz = EFI_PAGES_TO_SIZE(PAGES);


  Status = EFI_SUCCESS;
  TestStatus("VarPolTest-Init", Status);
  TEST_STOP("VarPolTest-Init");
  return Status;
}


/**
 *
 */
typedef EFI_STATUS (* VARPOLTESTCASE)(VOID);
VARPOLTESTCASE VarPolTests[] = {
  VarPolTestInit,
  VarPolTestDumpNullPolicies,
  VarPolTestSanity,
  VarPolTestNoLock,
  VarPolTestLockOnCreate,
  VarPolTestLockOnCreate2,
  VarPolTestLockNow,
  VarPolTestLockNow2,
  VarPolTestLockOnVarState,
  VarPolTestDumpPolicies,
  VarPolTestNamespaceWildcardLockOnVarState, /* wildcard lockonvarstate */
  VarPolTestHashWildcardLockNow,    /* wildcard containing hashes */
  VarPolTestNamespaceWildcardLockNow,  /* wildcard locknow */
  VarPolTestNamespaceWildcardLockOnCreate, /* wildcard lockoncreate */
  VarPolTestNamespaceWildcardLockOnCreate2,
  VarPolTestDumpPolicies,          /* with wildcard */
  VarPolTestLock
};


/**
 *
 */
EFI_STATUS
EFIAPI
VarPolTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  UINTN      NumTests = ARRAY_SIZE(VarPolTests);
  UINTN      Index    = 0;

  do {
    Status = VarPolTests[Index]();
    if(EFI_ERROR (Status)) {
      break;
    }
  } while (Index++ < NumTests-1);

  return Status;
}


