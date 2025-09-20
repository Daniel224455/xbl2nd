/** @file
  Platform BDS library definition. A platform can implement 
  instances to support platform-specific behavior.

Copyright (c) 2019 Qualcomm Technologies, Inc All rights reserved.<BR>  
Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                          
    
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __PLATFORM_BDS_LIB_H_
#define __PLATFORM_BDS_LIB_H_

#include <Protocol/GenericMemoryTest.h>
#include <Library/GenericBdsLib.h>

/**
  Perform the memory test base on the memory test intensive level,
  and update the memory resource.

  @param  Level         The memory test intensive level.

  @retval EFI_STATUS    Successfully test all the system memory, and update
                        the memory resource

**/
typedef
EFI_STATUS
(EFIAPI *BASEM_MEMORY_TEST)(
  IN EXTENDMEM_COVERAGE_LEVEL Level
  );

/**
  This routine is called to see if there are any capsules we need to process.
  If the boot mode is not UPDATE, then we do nothing. Otherwise, find the
  capsule HOBS and produce firmware volumes for them via the DXE service.
  Then call the dispatcher to dispatch drivers from them. Finally, check
  the status of the updates.

  This function should be called by BDS in case we need to do some
  sort of processing even if there is no capsule to process. We
  need to do this if an earlier update went away and we need to
  clear the capsule variable so on the next reset PEI does not see it and
  think there is a capsule available.

  @param BootMode                 The current boot mode

  @retval EFI_INVALID_PARAMETER   The boot mode is not correct for an update.
  @retval EFI_SUCCESS             There is no error when processing a capsule.

**/
typedef 
EFI_STATUS
(EFIAPI *PROCESS_CAPSULES)(
  IN EFI_BOOT_MODE BootMode
  );


#endif
