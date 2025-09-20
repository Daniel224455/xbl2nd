/** @file VariablePolicyInternal.h

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
#ifndef __VAR_POLICY_INTERNAL_H__
#define __VAR_POLICY_INTERNAL_H__

#include <Uefi.h>

/* variable policy implementation */
extern VARIABLE_POLICY_PROTOCOL mVariablePolicyProtocol;

/**
  initialize the variable policy engine
**/
EFI_STATUS
VariablePolicyInitialize(
  VOID
  );

#endif // __VAR_POLICY_INTERNAL_H__
