/** @file FwCommonHelpers.c
   
  Functions common to all FwCommonLib.

  Copyright (c) 2011, 2018 - 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/05/31     prema       Support to print GUID in two different formats
 2011/10/12     jthompso    Initial version

=============================================================================*/

#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FwCommonLib.h>
#include <Library/QcomLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SimpleFileSystem.h>
#include "FwUtil_Common.h"


/**
 * Used to fill a 
 * function pointer in 
 * the case where there is no 
 * applicable function.
 * 
 * 
 * @return EFI_STATUS - EFI_SUCCESS
 */
EFI_STATUS
DummyFunction(void)
{
    return EFI_SUCCESS;
}

/**
 * Wrapper to gBS->Allocatepool. 
 *  
 * Clears buffer returned by 
 * gBS->Allocatepool. 
 * 
 * 
 * @param PoolType - PoolType for AllocatPool 
 * @param Size -     Size of buffer
 * @param Buffer -   Buffer 
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
AllocateAndZero(
    IN EFI_MEMORY_TYPE  PoolType,
    IN UINTN            Size,
    OUT VOID            **Buffer
    )
{
    EFI_STATUS Status;
    Status = gBS->AllocatePool(PoolType,Size,Buffer);

    if(EFI_ERROR(Status))
    {
        return Status;
    }
    ZeroMem(*Buffer,Size );
    return EFI_SUCCESS;
};

/**
 * print guid
 *  
 * @param guid        - guid 
 * @param DebugLevel  - type of log to be used to print GUID
 * @param arrayFormat - TRUE  --> {########-####-####-####-############}
 *                      FALSE --> {0x########,0x####,0x####,{0x##,0x##,0x##,0x##,0x##,0x##,0x##,0x##}}
 * 
 * @return 
 */
VOID
FwCommonPrintGuid(
  IN EFI_GUID  guid,
  IN UINT32 DebugLevel,
  IN BOOLEAN arrayFormat
  )
{
  UINTN i = 0;

  if (arrayFormat == TRUE)
  {
    DEBUG((DebugLevel,"{0x%08x,0x%04x,0x%04x,{",guid.Data1,guid.Data2,guid.Data3));

    for (i = 0; i < 8; i++)
    {
      DEBUG((DebugLevel,"0x%02x",guid.Data4[i]));
      if(i!=7)
      {
        DEBUG((DebugLevel,","));
      }
    }
    DEBUG((DebugLevel,"}}"));
  }
  else
  {
    DEBUG((DebugLevel,"%08x-%04x-%04x-%02x%02x-",guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1]));

    for (i = 2; i < 8; i++)
    {
      DEBUG((DebugLevel,"%02x",guid.Data4[i]));
    }
  }

}

EFI_STATUS
FwUpdateBlockFileAlignFileBuffer2(
    IN EFI_MEMORY_TYPE  PoolType,
    IN UINTN            FileSize,
    IN VOID             *SrcBuffer,
    OUT VOID            **OutBuffer,
    IN UINTN            FileSizeAligned
    )
{
  EFI_STATUS Status            = EFI_SUCCESS;
  VOID*      FileBufferAligned = NULL;

  if((SrcBuffer == NULL) ||
     (OutBuffer == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }
  Status = AllocateAndZero(PoolType,
                           FileSizeAligned,
                           &FileBufferAligned);
  ERR_HANDLE(Status);

  if(SrcBuffer != NULL)
  {
    CopyMem(FileBufferAligned,SrcBuffer,FileSize);

  }
  *OutBuffer = FileBufferAligned;

  return Status;
    
}

EFI_STATUS
FwUpdateBlockFileAlignFileBuffer(
    IN EFI_MEMORY_TYPE  PoolType,
    IN UINTN            FileSize,
    OUT VOID            **FileBuffer,
    IN UINTN            FileSizeAligned
    )
{
  EFI_STATUS Status            = EFI_SUCCESS;
  VOID*      FileBufferAligned = NULL;

  if(FileBuffer == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  Status = AllocateAndZero(PoolType,
                           FileSizeAligned,
                           &FileBufferAligned);
  ERR_HANDLE(Status);

  if(*FileBuffer != NULL)
  {
    CopyMem(FileBufferAligned,*FileBuffer,FileSize);
    Status = gBS->FreePool(*FileBuffer); 
    ERR_HANDLE(Status);
  }
  *FileBuffer = FileBufferAligned;

  return Status;
    
}

/* Flush Variables services if supported else Skip further attempts to flush 
   assuming that flush operation is either not supported or not ready during
   early stage of development */
VOID
FlushVariableServicesIfSupported(
  VOID
  )
{
  
  static BOOLEAN skipVariableFlush = FALSE;
  EFI_STATUS     Status = EFI_SUCCESS;

  if (skipVariableFlush == FALSE){
     /* On successful write to variable services, trigger a flush
        to ensure it is written to disk without requring a reset. */
     Status = FlushVariableServices();
  
     if (EFI_NOT_FOUND == Status)
     {
       /* Skip further attempts to flush assuming that flush
          operation is either not supported or not ready during
          early stage of development */
       skipVariableFlush = TRUE;
       DEBUG((EFI_D_WARN,"FlushVariableServices returned %r. Will skip future calls to FlushVariableServices\n", Status));
     }
  }

}

