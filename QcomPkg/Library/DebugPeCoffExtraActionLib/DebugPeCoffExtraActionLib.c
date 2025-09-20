/**@file

Copyright (c) 2015 - 2018, Qualcomm Technologies Inc. All rights reserved.<BR>
Portions Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
Portions copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
Portions copyright (c) 2011 - 2012, ARM Ltd. All rights reserved.<BR>

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/PeCoffLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeCoffExtraActionLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ArmLib.h>
#include <Library/DepLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/CacheMaintenanceLib.h>

/* SYMPATH_ANCHOR_STR must be present in the Symbol path 
   Everything before SYMPATH_ANCHOR_STR is stripped from Symbol path */ 
#define SYMPATH_ANCHOR_STR    "Build"

/* UEFICORE_ANCHOR_STR is used to differentiate QC UEFI code */
#define UEFICORE_ANCHOR_STR   "boot_images"

/* Maximum size for T32 practice script */  
#define ALLOC_PAGES           8

/* Maximum size of string being added to T32 practice script */
#define TEMP_STR_SZ           1024

#define DRIVER_DELIMITER_CHAR ','
#define BKPT_DRVR_MAX_NUM     3
#define BKPT_DRVR_MAX_LEN     32

/* Format string should contain "0x" this is used for faster searching */
#define ADDRESS_FORMAT_STRING     "0x%012lx"

STATIC UefiInfoBlkType *InfoBlkPtr      = NULL;
STATIC CHAR8           *TempCmdString   = NULL;
CHAR8  DrvrList[BKPT_DRVR_MAX_NUM][BKPT_DRVR_MAX_LEN];
UINTN  DrvrNum                          = 0;
UINT8  DrvrListBuilt                    = 0;

UINTN  SpaceCeilingHit                  = 0;

/* Search SymbolPath for AnchorStr and strip everything before it 
   Return NULL if AnchorStr is not found, or if there is an error */
CHAR8 *
SanitizeForSymbolPath (
    IN  CHAR8   *SymbolPath, 
    IN  CHAR8   *AnchorStr,
    OUT UINTN* StripPartsPtr
	)
{
  CHAR8   *Ptr;
  UINTN    StripParts;
  CHAR8    TempPathStr[64];
  
  if (SymbolPath == NULL || AnchorStr == NULL)
    return NULL;
  
  /* Handle Windows/Linux path seperators */
  AsciiSPrint(TempPathStr, sizeof(TempPathStr), "/%a/", AnchorStr);
  Ptr = AsciiStrStr (SymbolPath, TempPathStr);
  if (Ptr == NULL) {
    AsciiSPrint(TempPathStr, sizeof(TempPathStr), "\\%a\\", AnchorStr);
    Ptr = AsciiStrStr (SymbolPath, TempPathStr);
  }

  if (StripPartsPtr)
  {
    StripParts = 0;
    while (SymbolPath < Ptr)
    {
       if ((*SymbolPath == '/') || (*SymbolPath == '\\'))
         ++StripParts;
       ++SymbolPath;
    }

    *StripPartsPtr = StripParts;
  }
    
  if (Ptr)
    return Ptr + 1;  /* Strip path sep */
  else
    return NULL;
}

CHAR8* ConvertToUpper (CHAR8* Str)
{
  CHAR8 *TempStr = Str;
  
  while (Str && *Str)
  {
    if ((*Str >= 'a') && (*Str <= 'z'))
      *Str = *Str - 0x20;
    ++Str;
  }

  return TempStr;
}

VOID
BuildDriverList(
  IN CONST CHAR8 *Drivers
  )
{
  CONST CHAR8 *Start, *End;

  if (Drivers == NULL)
    return;
  
  if (*Drivers == 0)
    return;

  Start = End = Drivers;
  //check for multiple drivers
  while (Start != NULL)
  {
    if ((*End == DRIVER_DELIMITER_CHAR) || (*End == '\0'))
    {
      CopyMem(DrvrList[DrvrNum], Start, ((UINTN)End - (UINTN)Start));
      Start = ++End;
      DrvrNum++;

      if (*End == '\0')
        break;
    }
    else
    {
      End++;
    }
  }

  DrvrListBuilt = 1;
}

CHAR8 *
EFIAPI
MatchDriverName(
  IN CONST CHAR8  *Command
  )
{
  CHAR8       *Rest = NULL;
  UINTN        Index =0;

  //Go up till DrvrNum so we don't compare empty string
  for (Index=0; Index < DrvrNum; Index++)
  {
    Rest = AsciiStrStr(Command, DrvrList[Index]);

    if (Rest)
      break;
  }

  return Rest;

}

/* Note that this function jumps backwards, ie we search from end of the
 * script backwards towards the beginning
 *
 * If input pointer is NULL, then take the pointer from InfoBlk ie a fresh
 * search, otherwise moves backwards from provided location, ie continue search
 * Returns the location of string 0x */
CHAR8* JumpToNextAddr (CHAR8* CurrentPtr)
{
  CHAR8 *ScriptStart;

  ScriptStart = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr;

  if (CurrentPtr)
    CurrentPtr--;

  while (CurrentPtr && (CurrentPtr > ScriptStart))
  {
    if ((CurrentPtr[0] == '0') && (CurrentPtr[1] == 'x'))
      return CurrentPtr;
    --CurrentPtr;
  }
  return NULL;
}

/* Search backwards for ".dll" and when found, go till either '/' or '\' */
CHAR8* GetModuleNameForAddr (CHAR8* AddrPtr, UINTN* LenPtr)
{
  CHAR8* ModulePtr = AddrPtr;
  UINTN  Len;
  CHAR8 *ScriptStart;

  if (AddrPtr == NULL)
    return NULL;

  ScriptStart = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr;
  
  while (ModulePtr && (ModulePtr > ScriptStart))
  {
    if ((ModulePtr[0] == '.') &&
        (ModulePtr[1] == 'd') &&
        (ModulePtr[2] == 'l') &&
        (ModulePtr[3] == 'l'))
      break;

    --ModulePtr;
  }

  if (ModulePtr == ScriptStart)
    return NULL;

  Len = 0;

  while (ModulePtr && (ModulePtr > ScriptStart))
  {
    if ((*ModulePtr == '/') ||
        (*ModulePtr == '\\'))
    {
      if (LenPtr)
        *LenPtr = Len;
      return ModulePtr + 1;
    }

    ++Len;
    --ModulePtr;
  }

  return NULL;
}

/* Logic is to search backwards till we find "&Sf=" and then
 * search forward till we find the same string. Then, move
 * everything after till the start of it and update the fields */
VOID RemoveCmdForModule (CHAR8* ModuleNameLocation)
{
  CHAR8 *CmdStart, *CmdEnd, *ScriptStart, *ScriptEnd, *Cptr;
  UINTN CmdShrinkSize;

  if (ModuleNameLocation == NULL)
    return;

  CmdStart = CmdEnd = ModuleNameLocation;
  ScriptStart = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr;
  ScriptEnd = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr + InfoBlkPtr->TotalCmdSize;

  while (CmdStart > ScriptStart)
  {
    if ((CmdStart[0] == '&') &&
        (CmdStart[1] == 'S') &&
        (CmdStart[2] == 'f') &&
        (CmdStart[3] == '='))
      break;

    --CmdStart;
  }

  while (CmdEnd < ScriptEnd)
  {
    if ((CmdEnd[0] == '&') &&
        (CmdEnd[1] == 'S') &&
        (CmdEnd[2] == 'f') &&
        (CmdEnd[3] == '='))
      break;

    ++CmdEnd;
  }

  /* Now we found the bounds, remove it. Keep it simple now, optimize later.
   * Removal algorith is,
   * Move the text from end of cmd till the end of script to start of cmd range
   * fill the end portion of script with nul for the length of
   * cmd removed */
  CmdShrinkSize = CmdEnd - CmdStart;

  CopyMem (TempCmdString, CmdStart, CmdShrinkSize);
  TempCmdString[CmdShrinkSize] = 0;

  Cptr = CmdStart;
  while (CmdEnd < ScriptEnd)
  {
    *Cptr = *CmdEnd;
    ++Cptr;
    ++CmdEnd;
  }

  while (Cptr < ScriptEnd)
  {
    *Cptr = 0;
    ++Cptr;
  }

  InfoBlkPtr->TotalCmdSize -= CmdShrinkSize;
}

/**
  Setup T32 practice script to enable host debug 

  If ImageContext is NULL, then return

  @param  ImageContext  Pointer to the image context structure that describes the
                        PE/COFF image that has already been loaded and relocated.

**/
VOID
EFIAPI
SetupDebugEnv (IN OUT PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext)
{
  CHAR8 *ImgPathPtr;
  CHAR8 *TempCmdPtr;
  UINTN  StripParts;
  CHAR8* CmdPtr;
  UINTN  CmdLen;
  UINT64 LoadAddr;

  if (ImageContext == NULL || ImageContext->PdbPointer == NULL)
    return;

  if (InfoBlkPtr == NULL)
  {
    InfoBlkPtr = (UefiInfoBlkType*)GetInfoBlkPtr();
    if (InfoBlkPtr == NULL)
      return;
  }

  if (TempCmdString == NULL)
  {
    /* Use Allocatepages EFI_PAGE_SIZE for TEMP_STR_SZ */
    TempCmdString = AllocatePages(1);
    if (TempCmdString == NULL)
      return;
  }

  if (InfoBlkPtr->SymbolLoadCmdStrPtr == 0)
  {
    InfoBlkPtr->LoaderBreakPoint = (UINT64)BreakPointFunction;
    InfoBlkPtr->SymbolLoadCmdStrPtr = (UINT64)AllocatePages(ALLOC_PAGES);

    if (InfoBlkPtr->SymbolLoadCmdStrPtr == 0)
      return;
    
    InfoBlkPtr->SymbolLoadCmdStrSize = ALLOC_PAGES * EFI_PAGE_SIZE;
    
    SetMem ((VOID*)InfoBlkPtr->SymbolLoadCmdStrPtr, InfoBlkPtr->SymbolLoadCmdStrSize, 0);

    /* AsciiSprint used later forces to \r\n for any combo of CR/LF */
    CmdPtr = AsciiStrCat ((CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr,
        "global &ModuleSrcRoot\r\nglobal &ModuleObjRoot\r\nlocal &Sf\r\n");

    CmdLen = AsciiStrLen (CmdPtr);
    InfoBlkPtr->TotalCmdSize += (UINT32)CmdLen;
  }

  CmdPtr = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr;
  LoadAddr = (ImageContext->ImageAddress + ImageContext->SizeOfHeaders);
  
  ImgPathPtr = SanitizeForSymbolPath(ImageContext->PdbPointer, UEFICORE_ANCHOR_STR, NULL);
  if (ImgPathPtr != NULL)
  {
    ImgPathPtr = SanitizeForSymbolPath(ImageContext->PdbPointer, SYMPATH_ANCHOR_STR, &StripParts);
    if (ImgPathPtr == NULL)
      return;

    /*
    * For each core module, add the following to the cmm script:
    * &Sf="<path_to_dll>"
    * if (os.file("&Sf"))
    * (
    *   data.load.elf "&Sf" <addr> /nocode /noclear /strippart <strip>. 
    * )
    */
    AsciiSPrint (TempCmdString, 
                 TEMP_STR_SZ, 
                 "&Sf=\"%a\"\nif (os.file(\"&Sf\"))\n(\n  data.load.elf \"&Sf\" "
                 ADDRESS_FORMAT_STRING
                 " /nocode /noclear /strippart %d.\n)\n",
                 ImgPathPtr, 
                 LoadAddr,
                 StripParts);
  }
  else
  {
    ImgPathPtr = SanitizeForSymbolPath(ImageContext->PdbPointer, SYMPATH_ANCHOR_STR, NULL);
    if (ImgPathPtr == NULL)
      return;

 /*
  * For each app pkg module, add the following to the cmm script:
  * &Sf="<path_to_dll>"
  * if (os.file("&Sf"))
  * (
  *   data.load.elf "&Sf" <addr> /nocode /noclear /strippart "&ModuleSrcRoot" /strippart "&ModuleObjRoot" /sourcepath "."
  * )
  */
    AsciiSPrint (TempCmdString, 
                 TEMP_STR_SZ, 
                 "&Sf=\"%a\"\nif (os.file(\"&Sf\"))\n(\n  data.load.elf \"&Sf\" "
                 ADDRESS_FORMAT_STRING
                 " /nocode /noclear /strippart \"&ModuleSrcRoot\" /strippart "
                 "\"&ModuleObjRoot\" /sourcepath \".\"\n)\n",
                 ImgPathPtr, 
                 LoadAddr);
  }

  CmdLen = AsciiStrLen (TempCmdString);

  /* Silently skip loading symbols if there is no more space */
  if (InfoBlkPtr->TotalCmdSize + CmdLen > InfoBlkPtr->SymbolLoadCmdStrSize)
  {
    ++SpaceCeilingHit;
    return;
  }
 
  TempCmdPtr = CmdPtr + InfoBlkPtr->TotalCmdSize;

  /* Write T32 practice script changes to memory */
  InfoBlkPtr->TotalCmdSize += (UINT32)CmdLen;
  CopyMem(TempCmdPtr, TempCmdString, CmdLen);

  if (!DrvrListBuilt)
    BuildDriverList(InfoBlkPtr->DebugStopDriverStr);

  /* Check if current module matches string passed by host and enter breakpoint */
  AsciiSPrint(TempCmdString, TEMP_STR_SZ, "%a", ImgPathPtr);
  if (MatchDriverName(ConvertToUpper(TempCmdString)))
  {
    ExtractModuleName (ImgPathPtr, InfoBlkPtr->BkptModuleName);
    BreakPointFunction();
  }
}

STATIC
VOID
DebugEnvUnload (IN PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext)
{
  CHAR8 *TempCmdPtr;
  CHAR8 *UnloadingModule;
  CHAR8 *AddrLocation;
  UINTN  CmdLen;
  CHAR8  AddressString[20];
  UINT64 LoadAddr;
  CHAR8 *ModuleNameLocation, *ScriptStart;
  UINTN  ModNameLen;
  CHAR8 *EndOfModulePtr, *StartOfModulePtr;

  if (ImageContext == NULL || ImageContext->PdbPointer == NULL)
    return;

  if (InfoBlkPtr == NULL)
  {
    InfoBlkPtr = (UefiInfoBlkType*)GetInfoBlkPtr();
    if (InfoBlkPtr == NULL)
      return;
  }

  if (InfoBlkPtr->TotalCmdSize == 0)
    return;

  ScriptStart = (CHAR8*)InfoBlkPtr->SymbolLoadCmdStrPtr;

  LoadAddr = (ImageContext->ImageAddress + ImageContext->SizeOfHeaders);

  AsciiSPrint (AddressString, sizeof(AddressString), ADDRESS_FORMAT_STRING, LoadAddr);

  UnloadingModule = ExtractModuleName(ImageContext->PdbPointer, TempCmdString);
  if (UnloadingModule == NULL)
    return;

  AddrLocation = TempCmdPtr = ScriptStart + InfoBlkPtr->TotalCmdSize;
  while (AddrLocation)
  {
    AddrLocation = JumpToNextAddr (AddrLocation);

    if (AddrLocation == NULL)
      break;

    if (CompareMem (AddressString, AddrLocation, AsciiStrLen(AddressString) - 1) == 0)
      break;
  }

  if (AddrLocation)
  {
    ModuleNameLocation = GetModuleNameForAddr (AddrLocation, &ModNameLen);

    if (ModuleNameLocation == NULL)
      return;

    if (CompareMem (ModuleNameLocation, UnloadingModule, ModNameLen - 1) == 0)
    {
      RemoveCmdForModule (ModuleNameLocation);
      return;
    }
  }
}

STATIC VOID 
PrintEfiImageInfo(PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext)
{
  UINTN Index;
  UINTN StartIndex;
  CHAR8 EfiFileName[256];

  DEBUG((EFI_D_WARN, " - 0x%9p [%5d] ",
        (VOID *) (UINTN) ImageContext->ImageAddress, GetTimerCountms ()));

  //
  // Print Module Name by Pdb file path.
  // Windows and Unix style file path are all trimmed correctly.
  //
  if (ImageContext->PdbPointer != NULL)
  {
    StartIndex = 0;
    for (Index = 0; ImageContext->PdbPointer[Index] != 0; Index++)
    {
      if ((ImageContext->PdbPointer[Index] == '\\') || (ImageContext->PdbPointer[Index] == '/'))
        StartIndex = Index + 1;
    }
    
    //
    // Copy the PDB file name to our temporary string, and replace .pdb with .efi
    // The PDB file name is limited in the range of 0~255.
    // If the length is bigger than 255, trim the redudant characters to avoid overflow in array boundary.
    //
    for (Index = 0; Index < sizeof (EfiFileName) - 4; Index++)
    {
      EfiFileName[Index] = ImageContext->PdbPointer[Index + StartIndex];

      if (EfiFileName[Index] == 0)
        EfiFileName[Index] = '.';

      if (EfiFileName[Index] == '.')
      {
        EfiFileName[Index + 1] = 'e';
        EfiFileName[Index + 2] = 'f';
        EfiFileName[Index + 3] = 'i';
        EfiFileName[Index + 4] = 0;
        break;
      }
    }

    if (Index == sizeof (EfiFileName) - 4)
      EfiFileName[Index] = 0;

    DEBUG ((EFI_D_WARN, "%a", EfiFileName)); // &ImageContext->PdbPointer[StartIndex]));
  }

  DEBUG ((EFI_D_WARN, "\n"));
}

/**
  Performs additional actions after a PE/COFF image has been loaded and relocated.

  If ImageContext is NULL, then ASSERT().

  @param  ImageContext  Pointer to the image context structure that describes the
                        PE/COFF image that has already been loaded and relocated.

**/
VOID
EFIAPI
PeCoffLoaderRelocateImageExtraAction (IN PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext)
{
  EFI_STATUS Status;

  if (ImageContext == NULL)
    return;

  PrintEfiImageInfo (ImageContext);

  SetupDebugEnv (ImageContext);

#ifdef ENABLE_DEP_64
  Status = SetupDep (ImageContext);
  {
    if (Status != EFI_SUCCESS)
      DEBUG ((EFI_D_ERROR, "ERROR: Failed to enable DEP\n"));
  }
#endif
}

/**
  Performs additional actions just before a PE/COFF image is unloaded.  Any resources
  that were allocated by PeCoffLoaderRelocateImageExtraAction() must be freed.
  
  If ImageContext is NULL, then ASSERT().
  
  @param  ImageContext  Pointer to the image context structure that describes the
                        PE/COFF image that is being unloaded.

**/
VOID
EFIAPI
PeCoffLoaderUnloadImageExtraAction (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext
  )
{
  EFI_STATUS Status;

  if ((ImageContext == NULL) ||  (ImageContext->ImageSize == 0x0))
  {
    DEBUG ((EFI_D_ERROR, "ERROR: Encountered NULL ImageContext\n"));
    return;
  }

#ifdef ENABLE_DEP_64
  Status = ClearDep (ImageContext);
  {
    if (Status != EFI_SUCCESS)
      DEBUG ((EFI_D_ERROR, "ERROR: Failed to enable DEP\n"));
  }
#endif  

  DebugEnvUnload (ImageContext);
}

