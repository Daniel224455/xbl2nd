/** @file FvDecompress.c

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
#include <Library/HobLib.h>

#include <Guid/ExtractSection.h>
#include <Library/LockLib.h>
#include <Library/KernelLib.h>
#include <Library/QcomBaseLib.h>

//#define ENABLE_LOCAL_DEBUG_LOGS 1

/*GUID for Decompression Data HOB {12DBD93D-402A-416E-EC20-5F80CF5F7FE7}*/
extern EFI_GUID gFvDecompressHobGuid;
EFI_GUID    *ThreadArgFvFileGuid = NULL;

/*Note:
  The list of FVs (DecompressFvList) is intended to be initialized once only
  in SEC and the pointer queried in later phases (DXE onwards) to get the
  status of FV processing started in SEC. Since this lib would get compiled
  in many drivers, initializing a new instance later is not preferred.
  DecompressFvHobInit should only be called in SEC. 
*/

EFI_STATUS
DisplayBootTime (CHAR8* Key, CHAR8* Event, BOOLEAN OnLCD)
{
  UINT64 Time;

  Time = GetTimerCountms ();
  DEBUG ((EFI_D_ERROR, "%a [%5lld] %a\n", Key, Time, Event));

  return EFI_SUCCESS;
}


static int DecompressThreadEntry( VOID* ARG)
{
  EFI_COMMON_SECTION_HEADER  *Section;
  VOID                       *DstBuffer;
  VOID                       *ScratchBuffer;
  UINT32                      AuthenticationStatus;  
  UINT32                      ScratchBufferSize;
  EFI_STATUS                  Status;  
  EFI_GUID    *FvFileGuid;

  UINT32 current_core = 0;
  thread_t *k_DCThread = thread_get_current();

#ifdef ENABLE_LOCAL_DEBUG_LOGS  
  DEBUG ((EFI_D_ERROR, "DecompressThreadEntry: Start \n")); 
  DisplayBootTime("DecompressThreadEntry start", "", FALSE);
#endif

  current_core = thread_get_curr_cpu(k_DCThread);   
  DEBUG ((EFI_D_WARN, "Current core for decompresion %d \n", current_core));


  FvDecompressMutexAcquire();
  FvFileGuid = ThreadArgFvFileGuid;
  FvDecompressMutexRelease();
  
  /*Setup Local Thread Variables */
  GetFvDecompressArgs(FvFileGuid, &DstBuffer, NULL, (VOID **)&Section,
                      &ScratchBuffer, &ScratchBufferSize, NULL);

  Status = SetFvProcessingStatus(FvFileGuid, FALSE);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: Failed to acquire FV mutex - %r : %g\n", Status, FvFileGuid));
    return (int) Status;    
  } 
  
  //
  // Call decompress function
  //
  Status = ExtractGuidedSectionDecode (
                Section,
                &DstBuffer,
                ScratchBuffer,
                &AuthenticationStatus
                );
          
  FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));

  /* Update Destination buffer start to skip 8 bytes. ExtractGuidedSectionDecode  
    returns meta info in first eight bytes and actual decompressed data starts after */
  DstBuffer = DstBuffer + 8;
  SetFvDecompressArgs(FvFileGuid, &DstBuffer, NULL, NULL,
                      NULL, NULL, NULL);
                      
  if (EFI_ERROR (Status))
  {
    //
    // Decompress failed
    //
    DEBUG ((EFI_D_ERROR, "Decompress Failed - %r\n", Status));
    return (int)EFI_NOT_FOUND;
  }

  Status = SetFvProcessingStatus(FvFileGuid, TRUE);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Error: Failed to release FV mutex - %r : %g\n", Status, FvFileGuid));
    return (int) Status;    
  } 

#ifdef ENABLE_LOCAL_DEBUG_LOGS  
  DEBUG ((EFI_D_ERROR, "DecompressThreadEntry: Done \n"));  
  DisplayBootTime("DecompressThreadEntry end", "", FALSE);
#endif
  
  return 0;
}

/*API For Thread setup, creation */
EFI_STATUS
LaunchFvDecompressThread ( VOID )
{
  thread_t  *FvThread;

  FvThread = NULL;

#ifdef ENABLE_LOCAL_DEBUG_LOGS
  DEBUG ((EFI_D_ERROR, " Start FvLaunch Thread \n"));
#endif

  FvThread = thread_create ("FvLaunch", &DecompressThreadEntry, (void*)0, DEFAULT_PRIORITY + 5, DEFAULT_STACK_SIZE);
  thread_resume (FvThread);

  return EFI_SUCCESS;
}

EFI_STATUS PerformGuidedSectionDecode (
    IN  EFI_COMMON_SECTION_HEADER  *FfsFileInfoSectionPtr,
    OUT VOID                       **OutputBuffer,
    OUT UINTN                      *OutSizePtr,
    IN  EFI_GUID                   *FvFileGuid,
    IN  BOOLEAN                    LaunchInSeperateThread    
  )
{
  EFI_STATUS                 Status;
  UINT32                     DstBufferSize = 0;
  VOID                       *ScratchBuffer;
  UINT32                     ScratchBufferSize;
  VOID                       *DstBuffer = NULL;
  UINT16                     SectionAttribute;
  EFI_COMMON_SECTION_HEADER  *Section;

  if ((OutputBuffer == NULL) || (OutSizePtr == NULL) || (FfsFileInfoSectionPtr == NULL))
    return EFI_INVALID_PARAMETER;

  Section = (EFI_COMMON_SECTION_HEADER*)FfsFileInfoSectionPtr;

  *OutputBuffer = NULL;
  Status = EFI_NOT_FOUND;

  if (Section->Type != EFI_SECTION_GUID_DEFINED)
    return EFI_INVALID_PARAMETER;
  
  Status = ExtractGuidedSectionGetInfo (Section,
                                        &DstBufferSize,
                                        &ScratchBufferSize,
                                        &SectionAttribute);
  if (EFI_ERROR (Status))
  {
    //
    // GetInfo failed
    //
    DEBUG ((EFI_D_ERROR, "Decompress GetInfo Failed - %r\n", Status));
    return EFI_NOT_FOUND;
  }
  //
  // Allocate destination buffer, extra one page for adjustment
  //
  DstBuffer = (VOID *)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (DstBufferSize)+1);
  if (DstBuffer == NULL)
    return EFI_OUT_OF_RESOURCES;
  
  //
  // DstBuffer still is one section. Adjust DstBuffer offset, skip EFI section header
  // to make section data at page alignment.
  // For compressed sections there is a Raw section header as well alongwith FV type section
  // so account for that too
  DstBuffer = (UINT8 *)DstBuffer + EFI_PAGE_SIZE - (sizeof (EFI_COMMON_SECTION_HEADER) * 2);
      
  //
  // Allocate scratch buffer
  //
  ScratchBuffer = (VOID *)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (ScratchBufferSize));
  if (ScratchBuffer == NULL)
    return EFI_OUT_OF_RESOURCES;

#ifdef ENABLE_LOCAL_DEBUG_LOGS        
       DEBUG ((EFI_D_ERROR, "Adding HOB Data for : %g\n", FvFileGuid));       
#endif
   
   /*Hob Creation has to happen before thread launch in SEC, if multithreaded*/ 
   Status = AddDecompressFvInfo( FvFileGuid);
   if(Status != EFI_SUCCESS)
   {
      DEBUG ((EFI_D_ERROR, "ERROR: Unable to add to FV info HOB %g\n", FvFileGuid));
      return Status;         
   }

  /*Initialize Thread Args */
  SetFvDecompressArgs(FvFileGuid, &DstBuffer, &DstBufferSize, (VOID **)&Section,
                      &ScratchBuffer, &ScratchBufferSize, NULL);

  FvDecompressMutexAcquire();                      
  ThreadArgFvFileGuid = FvFileGuid;  
  FvDecompressMutexRelease();
  
  if(LaunchInSeperateThread == TRUE)
  {
    LaunchFvDecompressThread();
  }
  else
  {
    Status = (EFI_STATUS) DecompressThreadEntry((void*)0);
    if(Status != EFI_SUCCESS)
    {
      DEBUG ((EFI_D_ERROR, "ERROR: DecompressThreadEntry failed for %g\n", FvFileGuid));
      return Status;         
   }    
  }

  if (OutSizePtr)
    *OutSizePtr = DstBufferSize;
    *OutputBuffer = DstBuffer + 8;
  
  
#ifdef ENABLE_LOCAL_DEBUG_LOGS
  DEBUG ((EFI_D_ERROR, " Decompress Complete - %p\n", DstBuffer));
#endif

  return EFI_SUCCESS;
}

/**
  This api enables decompression and mounting of additional firmware volumes
  from main fv.

  @param  Buffer                Pointer to the firmware volume to look into. Usually
                                Boot Firmware Volume (BFV).
  @param  BufferSize            Size of buffer containing source FV.
  @param  FvFileGuid            GUID of the FV to be searched, decompressed and mounted.

  @retval EFI_SUCCESS           The volume was found else error.

**/
EFI_STATUS
ProcessFvDecompression (
    IN  VOID        *Buffer,
    OUT UINTN        BufferSize,
    IN  EFI_GUID    *FvFileGuid,
    IN  BOOLEAN     LaunchInSeperateThread
  )
{
  FVContext*     FvCtxt;
  FfsFileInfo    FfsFileInfo, *FfsFileInfoPtr;
  VOID *OutBuffer;
  UINTN OutSize  = 0, ActualFvSize  = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  if (Buffer == NULL || BufferSize == 0 || FvFileGuid == NULL)
    return EFI_INVALID_PARAMETER;

  FvCtxt = OpenFVFile (Buffer, BufferSize, &ActualFvSize);

  if (FvCtxt == NULL)
    return EFI_INVALID_PARAMETER;

  FfsFileInfoPtr = GetNextFileFromFV (FvCtxt, &FfsFileInfo, 0);

  while (FfsFileInfoPtr)
  {
#ifdef ENABLE_LOCAL_DEBUG_LOGS
    DEBUG ((EFI_D_ERROR, "File : %g\n", &FfsFileInfoPtr->FileNameGuid));
    DEBUG ((EFI_D_ERROR, "File Size : %d (0x%X)\n", FfsFileInfoPtr->FileSize, FfsFileInfoPtr->FileSize));
    DEBUG ((EFI_D_ERROR, "File Type : %x\n", FfsFileInfoPtr->FileType));
    DEBUG ((EFI_D_ERROR, "File UI Name : %a\n", FfsFileInfoPtr->UIFileName));
    DEBUG ((EFI_D_ERROR, "File Ptr : %p\n\n", FfsFileInfoPtr->ROFileDataPtr));
#endif

    if (FfsFileInfoPtr->Attributes & FFS_FILE_ATTRIBUTE_COMPRESSED)
    {
      if (CompareGuid (FvFileGuid, &FfsFileInfoPtr->FileNameGuid))
      {

       Status = PerformGuidedSectionDecode ((EFI_COMMON_SECTION_HEADER  *)FfsFileInfoPtr->SectionPtr,
                                            &OutBuffer,
                                            &OutSize,
                                            FvFileGuid,
                                            LaunchInSeperateThread);

       if(Status != EFI_SUCCESS)
       {
          DEBUG ((EFI_D_ERROR, "ERROR: Unable to decompress Fv: %g\n", &FfsFileInfoPtr->FileNameGuid, FvFileGuid));
          return Status;
       }

       CloseFVFile (FvCtxt);
       return EFI_SUCCESS;
      }
    }
    FfsFileInfoPtr = GetNextFileFromFV (FvCtxt, &FfsFileInfo, 0);
  }

#ifdef ENABLE_LOCAL_DEBUG_LOGS
  DEBUG ((EFI_D_ERROR, "ProcessFvDecompression: Done \n"));
#endif

  CloseFVFile (FvCtxt);

  return EFI_NOT_FOUND;
}

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
)
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  VOID                    *DecompressFvList, **FvListPtr ;
  EFI_STATUS Status;
  
  /* Retrieve GUID HOB data that contains the FV buffer info. */
  GuidHob = GetFirstGuidHob (&gFvDecompressHobGuid);  
  
  if (GuidHob == NULL) 
    return EFI_NOT_FOUND;
  
  FvListPtr = GET_GUID_HOB_DATA (GuidHob);
  DecompressFvList = *FvListPtr;

  Status = ReInitFvListPostSec(DecompressFvList);

  if (EFI_ERROR (Status))
    DEBUG ((EFI_D_ERROR, "Error: ReInitFvListPostSec Failed - %r\n", Status));

  return Status;
 
}

