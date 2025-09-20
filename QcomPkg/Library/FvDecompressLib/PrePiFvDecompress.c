/** @file PrePiFvDecompress.c

  Source for decompressing FV in pre PI environment

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
#include <Library/PrePiFvDecompressLib.h>
#include <Library/FvList.h>

#include <Pi/PiFirmwareFile.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>

#include <Guid/ExtractSection.h>

//---------------------------
#include <Pi/PiPeiCis.h>
#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/PrePiLib.h>
//---------------------------

extern
EFI_STATUS
InitMemoryMutex ( 
  VOID
 );

extern
VOID *
EFIAPI
BuildGuidDataHob (
  IN CONST EFI_GUID              *Guid,
  IN VOID                        *Data,
  IN UINTN                       DataLength
  );

// #define ENABLE_LOCAL_DEBUG_LOGS 1

/*GUID for Decompression Data HOB {12DBD93D-402A-416E-EC20-5F80CF5F7FE7}*/
extern EFI_GUID gFvDecompressHobGuid;

/**
  Api to build FV Hob for Dxe consumption

  @param  FvGuid            GUID of the FV whose HOB needs to be added.

  @retval EFI_SUCCESS       Operation was success else error code.


**/
EFI_STATUS
AddFvInfoToHob(
  IN EFI_GUID  *FvGuid
  )
{
  EFI_FV_INFO    FvImageInfo;
  EFI_STATUS     Status;  
  UINT64  TempBuffAddr =0; 
  UINT64  TempBuffSize =0;  
  
  if(FvGuid == NULL)
     return EFI_INVALID_PARAMETER;

  Status = GetFvBufferInfo(FvGuid, &TempBuffAddr, &TempBuffSize);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: GetFvBufferInfo Failed - %r\n", Status));
    return Status;    
  } 
 
  FfsGetVolumeInfo ((EFI_PEI_FV_HANDLE) TempBuffAddr, &FvImageInfo);
  
  /* Inform HOB consumer phase, i.e. DXE core, the existance of this FV  */
  BuildFvHob ((EFI_PHYSICAL_ADDRESS) (UINTN) FvImageInfo.FvStart, FvImageInfo.FvSize);

  /* Make the encapsulated volume show up in DXE phase to skip processing of
    encapsulated file again.
  */
  BuildFv2Hob (
    (EFI_PHYSICAL_ADDRESS) (UINTN) FvImageInfo.FvStart,
    FvImageInfo.FvSize,
    &FvImageInfo.FvName,
    &(((EFI_FFS_FILE_HEADER *) TempBuffAddr)->Name)    // FvFileHandle
    );

  return EFI_SUCCESS;
}


/* API to initialize the List and Data structures keeping track of FVs under process */
EFI_STATUS 
DecompressFvHobInit ( 
  VOID 
)
{
  STATIC BOOLEAN HobInitialized = FALSE;  
  EFI_STATUS Status = EFI_SUCCESS;  

  VOID *DataPtr = NULL;  
  
  if(HobInitialized == TRUE)
    return EFI_SUCCESS;

  HobInitialized = TRUE;

  Status = DecompressFvListInit(&DataPtr);

  if(Status != EFI_SUCCESS )
    return Status;
  
  if(DataPtr !=NULL)
  {
    /*Since HOB cannot be updated later in DXE, we do not pass the whole data struct
      rather just the pointer to list of FV's under process 
    */
    BuildGuidDataHob (
      &gFvDecompressHobGuid,
      &DataPtr,
      sizeof (VOID *) );

    return EFI_SUCCESS;
  }
  
  return EFI_ABORTED; 
}

/**
   API to initialize Fv Decompress Lib and dependencies.

  @param  None

  @retval EFI_SUCCESS           Initialization was success else error.

**/
EFI_STATUS 
DecompressFvLibInit ( 
  VOID 
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*Signal memory allocation api's to synchronize using mutex
    from this point onwards, since multiThreaded decompress Fv lib api's
    depend on them.
  */
  InitMemoryMutex();
    
  /*Setup HOB data to pass on FV buffer info */
  Status = DecompressFvHobInit();
  
  return Status;
}

