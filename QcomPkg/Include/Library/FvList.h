/** @file FvDecompressHob.h

  Header file for decompress FV HOB info

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

#ifndef _FV_LIST_H_
#define _FV_LIST_H_

/**
  Api to create and initialize FV list
  

  @param  FvEntryPtr            Entry pointer if present else NULL.
  @param  FvFileGuid            GUID of the FV to be searched.

  @retval EFI_SUCCESS           Operation was success else error code.
          EFI_NOT_READY         If list of FV's is not maintained. 

**/
EFI_STATUS 
DecompressFvListInit ( 
  OUT VOID **DataPtr
);

/**
  Api adds an entry for input FV being processed. The EFI Guid
  is an ID used to create and locate the FV entry.

  @param  EFI_GUID            Guid of FV whos entry is to be created.
  
  @retval EFI_SUCCESS           Operation was success else error code.

**/ 
EFI_STATUS 
AddDecompressFvInfo (
  IN  EFI_GUID    *FvFileGuid
);

/**
  Api to wait till FV corresponding to input GUID name is decompressed/processed
  for further use. Is a blocking api.

  @param  FvFileGuid            GUID of the FV whose processing status is being queried

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
WaitTillProcessingComplete (
  IN  EFI_GUID    *FvFileGuid
);

/**
  Api to updates processing status of FV corresponding to input  
  GUID name. A TRUE indicates FV has been processed and mutex is released
  whereas FALSE indicates FV decompression in process, acquire mutex.

  @param  FvFileGuid            GUID of the FV whose processing status 
                                is to be updated
  @param  IsReady               Boolean flag status

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
SetFvProcessingStatus (
  IN  EFI_GUID   *FvFileGuid,
  IN BOOLEAN     IsReady  
);

/**
  This api reinitializes Data struct that keeps track of all FV lists under process.
  To be used post SEC.

  @param  FvListPtr     pointer to original list of FVs under process

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
ReInitFvListPostSec (
  IN VOID  *FvListPtr
);

/**
  This api retrieve's Buffer info for FV corresponding to input  
  GUID name.

  @param  FvFileGuid            GUID of the FV whose Buffer info 
                                is to be updated
  @param  FvBufferAddr          Start Address of Buffer containing FV
  @param  FvBufferSize          Size of Buffer

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
GetFvBufferInfo (
  IN EFI_GUID   *FvFileGuid,
  OUT UINT64     *FvBufferAddr,
  OUT UINT64     *FvBufferSize  
);

/**
  Api to save arguments to be passed on to FV decompression module.
  Only NonNull parameters are updated

  @param  *Section              Section address
  @param  *DstBuffer            FV Buffer
  @param  *ScratchBuffer        buffer used by decompression module
  @param  AuthenticationStatus  Saved status of decompression operation
  @param  ScratchBufferSize     Temporary buffer size
  @param  *FvFileGuid           GUID of the FV under process

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
SetFvDecompressArgs (
  IN EFI_GUID  *FvFileGuid,           
  IN VOID      **DstBuffer             OPTIONAL,
  IN UINT32    *DstBufferSize          OPTIONAL,  
  IN VOID      **Section               OPTIONAL,
  IN VOID      **ScratchBuffer         OPTIONAL,  
  IN UINT32    *ScratchBufferSize      OPTIONAL,
  IN UINT32    *AuthenticationStatus   OPTIONAL    
);

/**
  Api to retrieve arguments to be passed on to FV decompression module.
  Only NonNull parameters are updated

  @param  *Section              Section address
  @param  *DstBuffer            FV Buffer
  @param  *ScratchBuffer        buffer used by decompression module
  @param  AuthenticationStatus  Saved status of decompression operation
  @param  ScratchBufferSize     Temporary buffer size
  @param  *FvFileGuid           GUID of the FV under process

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
GetFvDecompressArgs (
  IN  EFI_GUID  *FvFileGuid,           
  OUT VOID      **DstBuffer             OPTIONAL,
  OUT UINT32    *DstBufferSize          OPTIONAL,  
  OUT VOID      **Section               OPTIONAL,
  OUT VOID      **ScratchBuffer         OPTIONAL,  
  OUT UINT32    *ScratchBufferSize      OPTIONAL,  
  OUT UINT32    *AuthenticationStatus   OPTIONAL
);


/**
  Api to acquire global mutex before updating any FV decompression Lib
  variable that might get trampled in multithreaded operation

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
FvDecompressMutexAcquire(
  VOID
);

/**
  Api to release global mutex after updating any FV decompression Lib
  variable that might get trampled in multithreaded operation

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
FvDecompressMutexRelease(
  VOID
  );
  
#endif
