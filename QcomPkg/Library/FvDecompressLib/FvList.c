/** @file FvList.c

  Source for maintaining info of FVs under process

  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All rights reserved.

**/


/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/04/19   kpa     Initial revision

=============================================================================*/

#include "com_dtypes.h"

#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#include "StWrappers.h"

#include <Library/FvParserLib.h>
#include <Library/FvList.h>

#include <Pi/PiFirmwareFile.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>

#include <Guid/ExtractSection.h>

// #define ENABLE_LOCAL_DEBUG_LOGS 1

#define MAX_FV_PROCESS_ENTRIES    4

typedef struct {
    UINT64         BufferAddr; 
    UINT64         BufferSize;
    mutex_t        *MutxPtr;
    EFI_GUID       ImageGuid;
    VOID           *Section;
    VOID           *ScratchBuffer;
    UINT64         ScratchBufferSize;     
    UINT32         AuthenticationStatus;
} FV_ENTRY_INFO;

typedef struct {
  UINT32         FvsToProcess;
  mutex_t        *DecomprsLibMutxPtr;  
  FV_ENTRY_INFO  FvBuff[MAX_FV_PROCESS_ENTRIES];
} FVS_DECOMPRESS_INFO;

STATIC FVS_DECOMPRESS_INFO *DecompressFvList = NULL;


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
)
{
  UINT32 Index = 0;
  
  if (DataPtr == NULL)
    return EFI_INVALID_PARAMETER;  
  
  DecompressFvList =  (FVS_DECOMPRESS_INFO*)AllocateZeroPool(sizeof(FVS_DECOMPRESS_INFO));  
  
#ifdef ENABLE_LOCAL_DEBUG_LOGS      
      DEBUG ((EFI_D_ERROR, "DecompressFvList Addr  : 0x%x\n", DecompressFvList));
#endif   

  if(DecompressFvList !=NULL)
  {
	DecompressFvList->DecomprsLibMutxPtr = init_mutex(0xBA5E);
    DecompressFvList->FvsToProcess = 0;
    while(Index < MAX_FV_PROCESS_ENTRIES)
    {
      DecompressFvList->FvBuff[Index].MutxPtr = init_mutex((Index << 0x2));
      Index++;
    }
    
    *DataPtr = (VOID *) DecompressFvList;
    return EFI_SUCCESS;
  }
  
  return EFI_OUT_OF_RESOURCES;
}
 
/**
  Api to return pointer to an entry in a list for FV under process
  if present

  @param  FvEntryPtr            Entry pointer if present else NULL.
  @param  FvFileGuid            GUID of the FV to be searched.

  @retval EFI_SUCCESS           Operation was success else error code.
          EFI_NOT_READY         If list of FV's is not maintained. 

**/
STATIC
EFI_STATUS 
GetDecompressFvEntry (
  IN  EFI_GUID    *FvFileGuid,
  OUT FV_ENTRY_INFO **FvEntryPtr
)
{
  UINT32 Index = 0;
  
  if(DecompressFvList == NULL)
    return  EFI_NOT_READY;
  
  if(FvFileGuid == NULL || FvEntryPtr == NULL )
    return  EFI_INVALID_PARAMETER;

  for(Index =0; Index < MAX_FV_PROCESS_ENTRIES; Index++)
  {
    if (CompareGuid (FvFileGuid, &(DecompressFvList->FvBuff[Index].ImageGuid)))
    {
#ifdef ENABLE_LOCAL_DEBUG_LOGS      
      DEBUG ((EFI_D_ERROR, "FV Entry Found  : %g\n", &(DecompressFvList->FvBuff[Index].ImageGuid)));
#endif      
      *FvEntryPtr = &DecompressFvList->FvBuff[Index];
      return EFI_SUCCESS;
    }
  }
  
  return EFI_NOT_FOUND;
}

/**
  Api adds an entry for input FV being processed. The EFI Guid
  is an ID used to create and locate the FV entry.

  @param  EFI_GUID            Guid of FV whos entry is to be created.
  
  @retval EFI_SUCCESS           Operation was success else error code.

**/ 
EFI_STATUS 
AddDecompressFvInfo (
  IN  EFI_GUID    *FvFileGuid
)
{
  UINT32 Index = 0;
  
  if(DecompressFvList == NULL)
    return  EFI_NOT_READY;
  
  if(FvFileGuid == NULL)
    return  EFI_INVALID_PARAMETER;
  
  if(DecompressFvList->FvsToProcess >= MAX_FV_PROCESS_ENTRIES)
    return EFI_BUFFER_TOO_SMALL;
  
  Index = DecompressFvList->FvsToProcess++;
  CopyGuid(&(DecompressFvList->FvBuff[Index].ImageGuid), FvFileGuid);
      
  return EFI_SUCCESS;
}
   
/**
  This api waits till FV corresponding to input GUID name is decompressed/processed
  for further use. Is a blocking api.

  @param  FvFileGuid            GUID of the FV whose processing status is being queried

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
WaitTillProcessingComplete (
  IN  EFI_GUID    *FvFileGuid
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  FV_ENTRY_INFO    *FvEntryPtr = NULL;

  if(FvFileGuid == NULL)
    return  EFI_INVALID_PARAMETER;
  
  Status = GetDecompressFvEntry(FvFileGuid, &FvEntryPtr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetDecompressFvEntry Failed - %r\n", Status));
    return Status;    
  } 
  
  /*Block on Fv entry mutex */
  mutex_acquire(FvEntryPtr->MutxPtr);
  
  mutex_release(FvEntryPtr->MutxPtr);  
  return EFI_SUCCESS;  
} 

/**
  This api updates processing status of FV corresponding to input  
  GUID name. A TRUE indicates FV has been processed, is ready
  else FALSE

  @param  FvFileGuid            GUID of the FV whose processing status 
                                is to be updated
  @param  IsReady               Boolean flag status

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
SetFvProcessingStatus (
  IN  EFI_GUID   *FvFileGuid,
  IN BOOLEAN     IsReady  
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  FV_ENTRY_INFO    *FvEntryPtr = NULL;

  if(FvFileGuid == NULL )
    return  EFI_INVALID_PARAMETER;
  
  Status = GetDecompressFvEntry(FvFileGuid, &FvEntryPtr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetDecompressFvEntry Failed - %r\n", Status));
    return Status;    
  } 

  if(IsReady == FALSE)
  {
    mutex_acquire(FvEntryPtr->MutxPtr);    
  }
  else
  {
    mutex_release(FvEntryPtr->MutxPtr);    
  }

  return EFI_SUCCESS;  
}

/**
  This api reinitializes Data struct that keeps track of all FV lists under process.
  To be used post SEC.

  @param  FvListPtr     pointer to original list of FVs under process

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
ReInitFvListPostSec (
  IN VOID  *FvListPtr
)
{
  if(FvListPtr == NULL)
    return EFI_INVALID_PARAMETER;
  
  DecompressFvList = (FVS_DECOMPRESS_INFO  *) FvListPtr;
  return EFI_SUCCESS;
}


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
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  FV_ENTRY_INFO    *FvEntryPtr = NULL;

  if(FvFileGuid == NULL || FvBufferAddr == NULL  || FvBufferSize == NULL  )
    return  EFI_INVALID_PARAMETER;
  
  Status = GetDecompressFvEntry(FvFileGuid, &FvEntryPtr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetDecompressFvEntry Failed - %r\n", Status));
    return Status;    
  } 
  
 *FvBufferAddr = FvEntryPtr->BufferAddr ;
 *FvBufferSize = FvEntryPtr->BufferSize ;  
  
  return EFI_SUCCESS;  
}

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
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  FV_ENTRY_INFO    *FvEntryPtr = NULL;

  if(FvFileGuid == NULL )
    return  EFI_INVALID_PARAMETER;
  
  Status = GetDecompressFvEntry(FvFileGuid, &FvEntryPtr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetDecompressFvEntry Failed - %r\n", Status));
    return Status;    
  } 
  
  if(DstBuffer != NULL)
    FvEntryPtr->BufferAddr            = (UINT64) *DstBuffer;
  
  if(DstBufferSize != NULL)
    FvEntryPtr->BufferSize            = *DstBufferSize;

  if(Section != NULL)
    FvEntryPtr->Section               = *Section;

  if(ScratchBuffer != NULL)
    FvEntryPtr->ScratchBuffer         = *ScratchBuffer;

  if(ScratchBufferSize != NULL)
    FvEntryPtr->ScratchBufferSize     = *ScratchBufferSize;

  if(AuthenticationStatus != NULL)
    FvEntryPtr->AuthenticationStatus  = *AuthenticationStatus;
  
  return EFI_SUCCESS;    
}

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
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  FV_ENTRY_INFO    *FvEntryPtr = NULL;

  if(FvFileGuid == NULL )
    return  EFI_INVALID_PARAMETER;
  
  Status = GetDecompressFvEntry(FvFileGuid, &FvEntryPtr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetDecompressFvEntry Failed - %r\n", Status));
    return Status;    
  } 
  
  if(DstBuffer != NULL)
    *DstBuffer = (VOID *)FvEntryPtr->BufferAddr;
  
  if(DstBufferSize != NULL)
    *DstBufferSize = FvEntryPtr->BufferSize;

  if(Section != NULL)
   *Section = FvEntryPtr->Section;
 
  if(ScratchBuffer != NULL)
    *ScratchBuffer = FvEntryPtr->ScratchBuffer;
 
  if(ScratchBufferSize != NULL)
    *ScratchBufferSize = FvEntryPtr->ScratchBufferSize;
 
  if(AuthenticationStatus != NULL)
    *AuthenticationStatus = FvEntryPtr->AuthenticationStatus;
  
  return EFI_SUCCESS;    
}

/**
  Api to acquire global mutex before updating any FV decompression Lib
  variable that might get trampled in multithreaded operation

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
FvDecompressMutexAcquire(
  VOID
  )
{
  if(DecompressFvList != NULL)
    mutex_acquire(DecompressFvList->DecomprsLibMutxPtr);
  
  return EFI_SUCCESS;
}

/**
  Api to release global mutex after updating any FV decompression Lib
  variable that might get trampled in multithreaded operation

  @retval EFI_SUCCESS           Operation was success else error.

**/
EFI_STATUS
FvDecompressMutexRelease(
  VOID
  )
{
  if(DecompressFvList != NULL)
    mutex_release(DecompressFvList->DecomprsLibMutxPtr);

  return EFI_SUCCESS;  
}