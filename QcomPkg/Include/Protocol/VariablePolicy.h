/** @file -- VariablePolicy.h

This protocol allows communication with Variable Policy Engine.

Copyright (c) 2019, Qualcomm Technologies, Inc.
Portions Copyright (c) 2018, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#ifndef __VARIABLE_POLICY_PROTOCOL__
#define __VARIABLE_POLICY_PROTOCOL__

#define VARIABLE_POLICY_PROTOCOL_REVISION   0x0000000000010000

#define VARIABLE_POLICY_PROTOCOL_GUID \
  { \
    0x81D1675C, 0x86F6, 0x48DF, { 0xBD, 0x95, 0x9A, 0x6E, 0x4F, 0x09, 0x25, 0xC3 } \
  }

#define VARIABLE_POLICY_ENTRY_REVISION      0x00010000

#pragma pack(push, 1)
typedef struct {
  //
  // Policy entry structure version
  //
  UINT32   Version;
  //
  // Size of the policy entry
  //
  UINT16   Size;
  //
  // Offset of the variable name field relative to the beginning of the policy
  // entry
  //
  UINT16   OffsetToName;
  //
  // GUID of the UEFI variable namespace
  //
  EFI_GUID Namespace;
  //
  // Minimum allowed size of the variable targeted by the policy entry
  //
  UINT32   MinSize;
  //
  // Maximum allowed size of the variable targeted by the policy entry
  //
  UINT32   MaxSize;
  //
  // Must-have attributes of the variable targeted by the policy entry
  //
  UINT32   AttributesMustHave;
  //
  // Can't-have attributes of the variable targeted by the policy entry
  //
  UINT32   AttributesCantHave;
  //
  // Type of locking, one of VARIABLE_LOCK_POLICY_TYPE
  //
  UINT8    LockPolicyType;
  //
  // Padding for LockPolicy alignment
  //
  UINT8    Padding[3];
  //
  // Lock policy structure
  //
  UINT8    LockPolicy[];
  //
  // Name of the variable targeted by the policy entry. Can be a zero-length
  // array (meaning all variables in the namespace are targeted) or can contain
  // "#" wildcards (meaning one numeric 0-9 character)
  //
  //CHAR16 Name[]
} VARIABLE_POLICY_ENTRY;

typedef enum {
  //
  // No locking is performed, only size and attribute enforcement. Means no
  // LockPolicy structure in the policy entry.
  //
  NoLock,
  //
  // Lock immediately. If the variable doesn't exist it won't be allowed to be
  // created. Means no LockPolicy structure in the policy entry.
  //
  LockNow,
  //
  // Lock after variable is created. Means no LockPolicy structure in the
  // policy entry.
  //
  LockOnCreate,
  //
  // Lock based on the state of another variable. Means that the LockPolicy is
  // of type VARIABLE_LOCK_ON_VAR_STATE_POLICY in the policy entry.
  //
  LockOnVarState,
  MaxLockPolicyType=255
} VARIABLE_LOCK_POLICY_TYPE;

typedef struct {
  //
  // GUID of the UEFI variable namespace
  //
  EFI_GUID Namespace;
  //
  // The value that specifies the state of the variable. If the variable specified
  // by Namespace and Name, matches the size (UINT8) and Value, the variable is locked.
  // If the size or value does not match, the variable is unlocked.
  //
  UINT8    Value;
  //
  // Padding for alignment
  //
  UINT8    Padding;
  //
  // Name of the variable. No wildcards here.
  //
  CHAR16   Name[];
} VARIABLE_LOCK_ON_VAR_STATE_POLICY;
#pragma pack(pop)

/**
  Disables the Variable Policy Engine, so that the present policy entries
  are no longer taken into account on variable access calls. This call
  effectively turns off the variable policy verification for this boot.
  This also disables UEFI Authenticated Variable protections including
  Secure Boot. DisableVariablePolicy can only be called once during boot.
  Note, this process is irreversible until the next system reset – there
  is no “EnablePolicy” protocol function.

  @retval EFI_SUCCESS          Successfully disabled variable policy.
  @retval EFI_ALREADY_STARTED  Variable policy already disabled.
**/
typedef
EFI_STATUS
(EFIAPI *DISABLE_VARIABLE_POLICY)(
  VOID
  );

/**
  Accepts a pointer to a Boolean in which it will store TRUE if Variable
  Policy Engine is enabled, or FALSE if Variable Policy Engine is disabled.

  @param State - variable where the output is stored

  @retval EFI_SUCCESS      Successfully retrieved the variable policy state.
**/
typedef
EFI_STATUS
(EFIAPI *IS_VARIABLE_POLICY_ENABLED)(
  OUT BOOLEAN *State
  );

/**
  Accepts a pointer to a policy entry structure and returns the status of
  policy registration. If the Variable Policy Engine is not locked and the
  policy structures are valid, the function will return EFI_SUCCESS. If the
  Variable Policy Engine is locked, RegisterVariablePolicy call will return
  EFI_WRITE_PROTECTED and will not register the policy entry.

  Upon successful registration of a policy entry, Variable Policy Engine will
  then evaluate this entry on subsequent variable access calls (as long as
  Variable Policy Engine hasn't been disabled).

  Wildcards are policies that:
   - Either do not specify a Name field in the policy entry. Therefore the policy
     targets the entire Namespace.
   - Or the Name field contains a '#' to denote a digit. If '#' wildcard is used,
     then Name must not contain any other digits.

  Order of precedence for matching variables to policy:
   - First check for non-wildcard policy match
   - Then match wildcard policy with all digits converted to '#'
   - Finally match wildcard policy targeting the Namespace

  @param PolicyEntry - pointer to a policy entry.

  @retval EFI_SUCCESS               Successfully registered a policy entry.
  @retval EFI_WRITE_PROTECTED       Variable Policy Engine is locked.
  @retval EFI_INCOMPATIBLE_VERSION  Policy Entry version is not supported.
  @retval EFI_ALREADY_STARTED       A Policy entry associated with Namespace and Name
                                      is already installed.
  @retval EFI_INVALID_PARAMETER     Invalid policy entry structure.

**/
typedef
EFI_STATUS
(EFIAPI *REGISTER_VARIABLE_POLICY)(
  IN VARIABLE_POLICY_ENTRY *PolicyEntry
  );

/**
  Accepts a pointer to a buffer and a pointer to the size of the buffer as
  parameters and returns the status of placing the policy into the buffer.
  On first call to DumpVariablePolicy one should pass NULL as the buffer and
  a pointer to 0 as the Size variable and DumpVariablePolicy will return
  EFI_BUFFER_TOO_SMALL and will populate the Size parameter with the size of
  the needed buffer to store the policy. This way, the caller can allocate
  the buffer of correct size and call DumpVariablePolicy again. The function
  will populate the buffer with policy and return EFI_SUCCESS.

  @param Policy - pointer to a buffer
  @param Size   - pointer to the size of the buffer or to the size of the
                  required buffer

  @retval EFI_SUCCESS             Successfully dumped the policy into the buffer.
  @retval EFI_BUFFER_TOO_SMALL    Insufficient buffer size.
**/
typedef
EFI_STATUS
(EFIAPI *DUMP_VARIABLE_POLICY)(
  IN OUT UINT8  *Policy,
  IN OUT UINT32 *Size
  );

/**
  locks the Variable Policy Engine, i.e. prevents any new policy entries from
  getting registered in this boot (RegisterVariablePolicy calls will fail with
  EFI_WRITE_PROTECTED status code returned).

  @retval EFI_SUCCESS      Successfully locked the Variable Policy Engine
**/
typedef
EFI_STATUS
(EFIAPI *LOCK_VARIABLE_POLICY)(
  VOID
  );

typedef struct {
  UINT64                     Revision;
  DISABLE_VARIABLE_POLICY    DisableVariablePolicy;
  IS_VARIABLE_POLICY_ENABLED IsVariablePolicyEnabled;
  REGISTER_VARIABLE_POLICY   RegisterVariablePolicy;
  DUMP_VARIABLE_POLICY       DumpVariablePolicy;
  LOCK_VARIABLE_POLICY       LockVariablePolicy;
} _VARIABLE_POLICY_PROTOCOL;

typedef _VARIABLE_POLICY_PROTOCOL VARIABLE_POLICY_PROTOCOL;

extern EFI_GUID gVariablePolicyProtocolGuid;

#endif
