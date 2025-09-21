/** @file ProcessCoalescedCapsules.h
   
  UEFI Capsule Service data structures.

  Copyright (c) 2012, 2015, 2017 Qualcomm Technologies, Inc. All rights reserved. 
Portions Copyright (c) 2004 - 2009, Intel Corporation. All 
rights 
reserved.<BR> This program and the accompanying materials 
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
 02/22/17   gparam  Renamed from CapsuleService.h and moved the header to capsule
                    processing library and moved some definitions to the public header
                    of the library
 03/05/15   jb      Add FMP support
 03/13/12   niting  Added firmware update support.
 02/06/12   jdt     Initial revision

=============================================================================*/

#ifndef __PROCESS_COALESCED_CAPSULES_H__
#define __PROCESS_COALESCED_CAPSULES_H__


/* CapsuleBuffer is the output of CapsuleDataCoalesce */
EFI_STATUS
EFIAPI
PersistCoalescedCapsules (
  IN VOID    *CapsuleBuffer
  );

/**

  This routine is called to process capsules which were 
  pushed by the previous boot. 
   
  Capsules with flag CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE set 
  will be installed to the system table, and those without will 
  be processed by ProcessCapsuleImage. 
   

  @param CapsuleBuffer            The Capsule Buffer to process. 
  @retval EFI_INVALID_PARAMETER   boot mode is not correct for an update
  @retval EFI_SUCCESS             There is no error when processing capsule

**/
EFI_STATUS
EFIAPI
ProcessCoalescedCapsules (
  VOID    *CapsuleBuffer
  );

#endif /* __PROCESS_COALESCED_CAPSULES_H__ */
