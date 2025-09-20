/** @file VariablePolicy.c

  Implements VARIABLE_POLICY_PROTOCOL

  Copyright (c) 2019 Qualcomm Technologies, Inc.  All rights reserved.
  Portions Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
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
 01/15/19   ai      Initial revision.

=============================================================================*/
#include <Protocol/VariablePolicy.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiCfgLib.h>
#include <Guid/GlobalVariable.h>
#include "VariablePolicyInternal.h"
#include "ScmCmdVar.h"

/* Change POL_DBG_LVL to DEBUG_WARN to enable additional debug output */
#define POL_DBG_LVL      DEBUG_INFO

#if defined (__GNUC__) || defined (__clang__)
  #define POL_DBG(Lvl, Str, ...) \
    DEBUG((Lvl, "<%a:%u> " Str "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__))
#else
  #define POL_DBG(Lvl, Str, ...)
#endif

/**
  @see VARIABLE_POLICY_PROTOCOL::DisableVariablePolicy
**/
STATIC
EFI_STATUS
EFIAPI
DisableVariablePolicy(
  VOID
  )
{
  return ScmCmdDisableVariablePolicy();
}

/**
  @see VARIABLE_POLICY_PROTOCOL::IsVariablePolicyEnabled
**/
STATIC
EFI_STATUS
EFIAPI
IsVariablePolicyEnabled(
  OUT BOOLEAN *State
  )
{
  EFI_STATUS Status    = EFI_INVALID_PARAMETER;
  UINT8      IsEnabled = 0;

  /* Sanity */
  if (State == NULL)
    return EFI_INVALID_PARAMETER;

  Status = ScmCmdIsVariablePolicyEnabled(&IsEnabled);
  if (Status == EFI_SUCCESS)
    *State = (IsEnabled == 1)? TRUE : FALSE;

  return Status;
}

/**
  @see VARIABLE_POLICY_PROTOCOL::RegisterVariablePolicy
**/
STATIC
EFI_STATUS
EFIAPI
RegisterVariablePolicy(
  IN VARIABLE_POLICY_ENTRY *PolicyEntry
  )
{
  /* Sanity */
  if (PolicyEntry == NULL) {
    POL_DBG(DEBUG_ERROR, "invalid arg");
    return EFI_INVALID_PARAMETER;
  }

  return ScmCmdRegisterVariablePolicy(PolicyEntry);
}


/**
  @see  VARIABLE_POLICY_PROTOCOL::DumpVariablePolicy
**/
STATIC
EFI_STATUS
EFIAPI
DumpVariablePolicy(
  IN OUT UINT8  *Policy,
  IN OUT UINT32 *Size
  )
{
  /* Sanity */
  if (Size == NULL ||
      (*Size > 0 && Policy == NULL))
  {
    POL_DBG(DEBUG_ERROR, "invalid arg");
    return EFI_INVALID_PARAMETER;
  }

  return ScmCmdDumpVariablePolicy(Policy, Size);
}


/**
  @see VARIABLE_POLICY_PROTOCOL::LockVariablePolicy
**/
STATIC
EFI_STATUS
EFIAPI
LockVariablePolicy(
  VOID
  )
{
  return ScmCmdLockVariablePolicy();
}


/*
 * Install pre-defiend variable policies
 */
STATIC
EFI_STATUS
InstallPredefinedVariablePolicies(
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  return Status;
}


VARIABLE_POLICY_PROTOCOL mVariablePolicyProtocol =
{
  VARIABLE_POLICY_PROTOCOL_REVISION,
  DisableVariablePolicy,
  IsVariablePolicyEnabled,
  RegisterVariablePolicy,
  DumpVariablePolicy,
  LockVariablePolicy
};

/**
  initialize the variable policy engine
**/
EFI_STATUS
VariablePolicyInitialize(
  VOID
  )
{
  EFI_STATUS  Status                              = EFI_UNSUPPORTED;
  BOOLEAN     IsVariablePolicyInitialized         = FALSE;
  UINT32      ConfigEnableVariablePolicyEngine    = 0;

  /* Check if non-secure world is okay to enable */
  Status = GetConfigValue("EnableVariablePolicyEngine", &ConfigEnableVariablePolicyEngine);
  if (Status != EFI_SUCCESS ||
      !(ConfigEnableVariablePolicyEngine & BIT_POLICY_ENGINE_NS_ENABLE)) {
    POL_DBG(DEBUG_WARN, "variable policy engine disabled");
    return EFI_NOT_READY;
  }

  /* Check if secure world is okay to enable */
  Status = IsVariablePolicyEnabled(&IsVariablePolicyInitialized);
  if ((Status != EFI_SUCCESS) || (IsVariablePolicyInitialized == FALSE)) {
    POL_DBG(DEBUG_WARN, "policy engine not supported in TZ");
    return EFI_NOT_READY;
  }

  Status = InstallPredefinedVariablePolicies();
  if (Status != EFI_SUCCESS)
    POL_DBG(DEBUG_ERROR, "failed to install pre-defined variable policies");

  return Status;
}
