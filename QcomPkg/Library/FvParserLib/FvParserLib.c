/** @file
  This file includes methods for Parsing FV in memory

  Copyright (c) 2020, Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  Portions copyright (c) 2004 - 2008, Intel Corporation. All rights reserved.
  
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 05/28/19   kpa       Created new file
=============================================================================*/


/*=========================================================================
      Include Files
==========================================================================*/
#include <Uefi.h>

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/ExtractGuidedSectionLib.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/FileSystemVolumeLabelInfo.h>
#include <Guid/LzmaDecompress.h>
#include "QcomLib.h"
#include <Library/GuidedFvLib.h>
//#include <Library/MountFvLib.h>
#include <Library/QcomBdsLib.h>
#include <MemRegionInfo.h>
#include <Library/UefiCfgLib.h>
#include <Library/UefiLib.h>
#include <Library/FvParserLib.h>

//#define ENABLE_LOCAL_DEBUG_LOGS 1

#define FV_CONTEXT_SIGNATURE    ('F' | ('v' << 8) | ('C' << 16) | ('t' << 24))

struct __FVContext
{
  UINT32 Signature;
  UINT32 Filler;
  VOID* FvBufferPtr;
  VOID* FvBufferEndPtr;
  VOID* CurrentFfsFilePtr;
  VOID* NextFfsFilePtr;
};

FVContext FvContexts[MAX_FV_CONTEXTS];
UINT8 FvCtxtStatus[MAX_FV_CONTEXTS];

VOID FreeFvContext (FVContext* Ctxt)
{
  int i;

  for (i = 0; i < MAX_FV_CONTEXTS; ++i)
  {
    if (&FvContexts[i] == Ctxt)
    {
      FvCtxtStatus[i] = 0;
      Ctxt->Signature = 0;
      Ctxt->Filler = 0;
      Ctxt->FvBufferPtr = 0;
      Ctxt->FvBufferEndPtr = 0;
      Ctxt->CurrentFfsFilePtr = 0;
      Ctxt->NextFfsFilePtr = 0;
      return;
    }
  }
}

FVContext* AllocateFvContext (VOID)
{
  int i;

  for (i = 0; i < MAX_FV_CONTEXTS; ++i)
  {
    if (FvCtxtStatus[i] == 0)
    {
      FvCtxtStatus[i] = 1;
      return &FvContexts[i];
    }
  }
  return NULL;
}

VOID CloseFVFile (FVContext* CtxtPtr)
{
  FreeFvContext (CtxtPtr);
}

FVContext* OpenFVFile (VOID * FVBuffer, UINTN BufferSize, UINTN* OPTIONAL FvSizePtr)
{
  EFI_FIRMWARE_VOLUME_HEADER     *FvHeader;
  VOID                           *FvEndPtr, *FfsFilePtr;
  FVContext                      *CtxtPtr;

  if (FVBuffer == NULL)
    return NULL;

  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER*)FVBuffer;

  if (FvHeader->Signature != EFI_FVH_SIGNATURE)
    return NULL;

  // Header is 8 bytes smaller than the struct size
  if ((FvHeader->HeaderLength > (2 * sizeof(EFI_FIRMWARE_VOLUME_HEADER))) ||
      (FvHeader->HeaderLength & 0x1))
    return NULL;

  if (CalculateSum16 ((UINT16*)FvHeader, FvHeader->HeaderLength) != 0)
    return NULL;

  FvEndPtr = (VOID*)((UINTN)FVBuffer + FvHeader->FvLength);

  if (FvHeader->FvLength > BufferSize)
    return NULL;

  CtxtPtr = AllocateFvContext ();

  if (CtxtPtr == 0)
    return NULL;

  if (FvSizePtr)
    *FvSizePtr = FvHeader->FvLength;

  // FfsFile Header is located after FV Header
  FfsFilePtr = (VOID*)((UINTN)FvHeader + (UINTN)FvHeader->HeaderLength);

  CtxtPtr->Signature = FV_CONTEXT_SIGNATURE;
  CtxtPtr->FvBufferPtr = FVBuffer;
  CtxtPtr->FvBufferEndPtr = FvEndPtr;
  CtxtPtr->CurrentFfsFilePtr = FfsFilePtr;
  CtxtPtr->NextFfsFilePtr = 0;

  return CtxtPtr;
}

FfsFileInfo* GetFfsFileInfo (EFI_FFS_FILE_HEADER  *FfsPtr, UINTN FfsFileHdrSize, VOID* FfsEndPtr, FfsFileInfo* FileInfoPtr)
{
   EFI_COMMON_SECTION_HEADER     *Section;
   UINT32 SectionSize, HeaderSize;

   // FFS header checksum already verified so should already be valid
   if (FfsFileHdrSize == sizeof (EFI_FFS_FILE_HEADER2))
     Section = (EFI_COMMON_SECTION_HEADER*)(((EFI_FFS_FILE_HEADER2*)FfsPtr) + 1);
   else
     Section = (EFI_COMMON_SECTION_HEADER*)(FfsPtr + 1);

   FileInfoPtr->FileNameGuid = FfsPtr->Name;
   FileInfoPtr->FileType = FfsPtr->Type;
   FileInfoPtr->Attributes = FfsPtr->Attributes;
   FileInfoPtr->UIFileName[0] = 0;
   FileInfoPtr->FileSize = 0;
   FileInfoPtr->ROFileDataPtr = NULL;
   FileInfoPtr->SectionPtr = NULL;

   while ((VOID*)Section < FfsEndPtr)
   {
     VOID* PayloadPtr;

     if (IS_SECTION2 (Section))
     {
       SectionSize = SECTION2_SIZE (Section);
       PayloadPtr = (VOID*)(((EFI_COMMON_SECTION_HEADER2 *)Section) + 1);
       HeaderSize = sizeof(EFI_COMMON_SECTION_HEADER2);
     }
     else
     {
       SectionSize = SECTION_SIZE (Section);
       PayloadPtr = (VOID*)(Section + 1);
       HeaderSize = sizeof(EFI_COMMON_SECTION_HEADER);
     }

#ifdef ENABLE_LOCAL_DEBUG_LOGS
     DEBUG ((EFI_D_ERROR, "Section %09lX Type - %x Size : %x\n", Section, Section->Type, SectionSize));
#endif

     switch (Section->Type)
     {
       case EFI_SECTION_USER_INTERFACE:
         {
           UINTN CopiedChars;
           UnicodeStrnToAsciiStrS ((CONST CHAR16*)PayloadPtr,  sizeof(FileInfoPtr->UIFileName) - 1, FileInfoPtr->UIFileName, sizeof(FileInfoPtr->UIFileName), &CopiedChars);
         }
         break;

       // Data payload types
       case EFI_SECTION_GUID_DEFINED:
         FileInfoPtr->Attributes |= FFS_FILE_ATTRIBUTE_COMPRESSED;
         // Continue to common code with rest of cases
       case EFI_SECTION_PE32:
       case EFI_SECTION_RAW:
       case EFI_SECTION_FIRMWARE_VOLUME_IMAGE:
          FileInfoPtr->FileSize = SectionSize - HeaderSize;
          FileInfoPtr->ROFileDataPtr = PayloadPtr;
          FileInfoPtr->SectionPtr = Section;
          break;

       case EFI_FV_FILETYPE_FFS_PAD:
          return NULL;

       case EFI_SECTION_DXE_DEPEX:
          break;

       case EFI_SECTION_TE:
           break;
           
       default:
        DEBUG ((EFI_D_ERROR, "Unknown section type - %X\n", Section->Type));
     }

     // Move to next section
     SectionSize = (SectionSize + 3) & ~3;
     Section = (EFI_COMMON_SECTION_HEADER*)((UINTN)Section + SectionSize);

     if (SectionSize == 0)
       return NULL;
   }

   return FileInfoPtr;
}

FfsFileInfo* GetNextFileFromFV (FVContext* Ctxt, FfsFileInfo* FileInfoPtr, UINT8 OPTIONAL GetFileType)
{
  EFI_FFS_FILE_HEADER  *FfsPtr;
  UINT32 FfsSize;
  VOID *FvEndPtr, *FfsEndPtr;
  UINT8 HeaderChecksum;
  UINTN HeaderSize;

  if ((Ctxt == 0) || (Ctxt->Signature != FV_CONTEXT_SIGNATURE) || (FileInfoPtr == 0))
    return NULL;

  // NOTE: Since operating on the source buffer of the container FV, don't
  // align the absolute value of pointer for FFS to 8, instead align the
  // byte offset wrt original FV address, which will have same effect as 
  // absolute pointer aligned to 8 bytes. This doesn't apply for a FV copied
  // out to new buffer

  // If we already traversed through, then advance to next file
  if (Ctxt->NextFfsFilePtr != 0)
    Ctxt->CurrentFfsFilePtr = Ctxt->NextFfsFilePtr;

  FfsPtr = Ctxt->CurrentFfsFilePtr;
  FvEndPtr = Ctxt->FvBufferEndPtr;

  while ((VOID*)FfsPtr < FvEndPtr)
  {
    VOID *FfsHdrEnd;

    FfsHdrEnd = (VOID*)((UINTN)FfsPtr + sizeof (EFI_FFS_FILE_HEADER));

    // Make sure the FFS header is still within the buffer limits
    if (FfsHdrEnd >= FvEndPtr)
      return NULL;

    if (IS_FFS_FILE2 (FfsPtr))
    {
      HeaderSize = sizeof (EFI_FFS_FILE_HEADER2);
      HeaderChecksum = CalculateSum8 ((UINT8 *) FfsPtr, HeaderSize);
      FfsSize = FFS_FILE2_SIZE (FfsPtr);
    }
    else
    {
      HeaderSize = sizeof (EFI_FFS_FILE_HEADER);
      HeaderChecksum = CalculateSum8 ((UINT8 *) FfsPtr, HeaderSize);
      FfsSize = FFS_FILE_SIZE (FfsPtr);
    }

    HeaderChecksum = (UINT8) (HeaderChecksum - FfsPtr->State - FfsPtr->IntegrityCheck.Checksum.File);

    if (HeaderChecksum != 0)
       return NULL;
    
    if ((FfsSize == 0) || (FfsPtr->Type == 0xFF))
      return NULL;

#ifdef ENABLE_LOCAL_DEBUG_LOGS
    DEBUG ((EFI_D_ERROR, "FFS type - %x, Sz : %X\n", FfsPtr->Type, FfsSize));
#endif

    FfsEndPtr = (VOID*)((UINTN)FfsPtr + FfsSize);

    if (FfsEndPtr > FvEndPtr)
      return NULL;

    // This serves the purpose of aligning the FFS to 8 bytes boundary as stated in above note
    FfsSize = (FfsSize + 7) & ~7;
    Ctxt->NextFfsFilePtr = (VOID*)((UINTN)FfsPtr + FfsSize);

    if ((FfsPtr->Type != EFI_FV_FILETYPE_FFS_PAD) && ((GetFileType == 0) || (FfsPtr->Type == GetFileType)))
    {
      Ctxt->CurrentFfsFilePtr = FfsPtr;

      // Got to the FFS file of interest
      // now process all section entries to get the FFS File info
      return GetFfsFileInfo (FfsPtr, HeaderSize, FfsEndPtr, FileInfoPtr);
    }

    // Goto next FFS File
    FfsPtr = (EFI_FFS_FILE_HEADER*)Ctxt->NextFfsFilePtr;
  }
  return NULL;
}

