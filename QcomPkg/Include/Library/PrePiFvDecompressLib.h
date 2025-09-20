/** @file PrePiFvDecompressLib.h

  Header file for decompressing FV in pre PI environment

  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 06/04/19   kpa     Initial revision
 
=============================================================================*/

#ifndef _PREIPIFVDECOMPRESSLIB_H_
#define _PREIPIFVDECOMPRESSLIB_H_

/**
  This api enables decompression and mounting of additional firmware volumes
  from main fv.

  @param  Buffer                Pointer to the firmware volume to look into. Usually
                                Boot Firmware Volume (BFV).
  @param  BufferSize            Size of buffer containing source FV.
  @param  FvFileGuid            GUID of the FV to be searched, decompressed and mounted.

  @retval EFI_SUCCESS           Operation was success else error.

**/

EFI_STATUS
ProcessFvDecompression (
  IN  VOID        *Buffer,
  OUT UINTN        BufferSize,
  IN  EFI_GUID    *FvFileGuid,
  IN  BOOLEAN     LaunchInSeperateThread
);

/**
  Api Build FV Hob for Dxe consumption

  @param  FvGuid            GUID of the FV whose HOB needs to be added.

  @retval EFI_SUCCESS           Operation was success else error code.


**/
EFI_STATUS
AddFvInfoToHob(
  IN EFI_GUID  *FvGuid
);
  
/**
   API to initialize Fv Decompress Lib and dependencies.
   Creates a Data HOB to pass on FV Decompression meta info beyond SEC

  @param  None

  @retval EFI_SUCCESS           Initialization was success else error.

**/
EFI_STATUS 
DecompressFvLibInit ( 
  VOID 
);

/**
   API to Reinitialize Fv Decompress Lib.
   This retrieves FV Data pointer shared by SEC via HOB and initializes
   post-SEC instance of the Lib

  @param  None

  @retval EFI_SUCCESS           Initialization was success else error.

**/
EFI_STATUS 
DecompressFvLibReInit ( 
  VOID 
);

#endif
