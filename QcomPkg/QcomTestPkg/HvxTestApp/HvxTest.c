/** @file HvxTest.c

  Tests for calling into HYP to launch HVX.

  Copyright (c) 2010-2011, 2015 Qualcomm Technologies Inc. All rights reserved.
  Portions Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.
  This program and the accompanying materials
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
 05/21/18   pweber  Fixes for SKTestApp launch failures
 05/26/16   pgw     Initial revision

=============================================================================*/

#include <Uefi.h>
#include <Library/ArmLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/TestInterface.h>
#include <Library/DevicePathLib.h>
#include <Include/scm_sip_interface.h>
#include <Protocol/EFIScm.h>
#include <Protocol/LoadFile.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePath.h>
#include "hyp_hvx_common.h"
#include <Library/PeCoffLib.h>
#include <Library/MemoryAllocationLib.h>
#include <string.h>

// Global defines
#define SK_BUFFER_SIZE 0x2000
#define SK_LOAD_ADDRESS 0x8AB00000
#define TZ_PSCI_MEM_PROTECT  0x84000013    

extern EFI_GUID gEfiACPITableLoadGuid;

static const UINT8 *LauncherImagePtr = (UINT8*)SK_LOAD_ADDRESS;
static UINT64 LauncherDataPtr = 0x0;
EFI_HANDLE gHvxTestHandle=NULL;
EFI_EVENT ACPITableLoadEvent  = NULL;

// Function prototype for assmembly code in smc_util.s
UINT64 ArmSMCCall(UINT64 param1, UINT64 param2, UINT64 param3);

// Deprecated test case for 8996, will remove this after we've finished
// validation of the rest of the path
#define TZBSP_SYSCALL_PARAM_TYPE_VAL              0x0
#define TZBSP_SYSCALL_PARAM_TYPE_BUF_RO           0x1
#define TZBSP_SYSCALL_PARAM_TYPE_BUF_RW           0x2
#define TZBSP_SYSCALL_PARAM_TYPE_BUF_VALIDATION   0x3

#define HYP_HVX_SECURE_HANDOFF_ID                  \
   TZ_SYSCALL_CREATE_SMC_ID(TZ_OWNER_SIP, TZ_SVC_BOOT, 0x40)

#define HYP_HVX_SECURE_HANDOFF_PARAM_ID            \
      TZ_SYSCALL_CREATE_PARAM_ID_6(TZBSP_SYSCALL_PARAM_TYPE_BUF_RO,   \
                                   TZBSP_SYSCALL_PARAM_TYPE_VAL,      \
                                   TZBSP_SYSCALL_PARAM_TYPE_BUF_RO,   \
                                   TZBSP_SYSCALL_PARAM_TYPE_VAL,      \
                                   TZBSP_SYSCALL_PARAM_TYPE_VAL,      \
                                   TZBSP_SYSCALL_PARAM_TYPE_VAL )

/**
  Wrapper to print out parameters and results from SMC call to HYP

  @retval EFI_SUCCESS       All test cases passed successfully
  @retval other             Error occurred when trying to launch image

**/
EFI_STATUS HypSKLaunch( SMC_PARAMETERS *params, SK_LAUNCH_ACTION action )
{
  EFI_STATUS status =  EFI_SUCCESS;
  
  // Setup the parameters that get passed into SMC call
  AsciiPrint("Scm Call --> 0x%x, 0x%x, 0x%x\n", HYP_SECURE_KERNEL_LAUNCH_ID,
                      (UINT64)params, action);
  
  // First Map all the memory we need for the call into EL2
  // Blocking call so disable interrupts as MSFT will have interrupts disabled
  ArmDisableInterrupts();
  status = (EFI_STATUS)ArmSMCCall(HYP_SECURE_KERNEL_LAUNCH_ID, (UINT64)params, action);
  // Re-enable interrupts to allow print statements and other things to happen
  ArmEnableInterrupts();

  if (status != EFI_SUCCESS) {
    AsciiPrint("ScmSipSysCall failed, Status = (0x%x)\r\n", status);
  } else {
    AsciiPrint("Successfully called to SK_LAUNCH_ACTION:0x%x\n", action);
  }

  return status;
}

/**
  Retrieve the memory map from UEFI and make a local copy that we can pass into
  the SK and HYP to validate and know the memory map for HLOS

  @param[in] attributePtr   A pointer to the launch attributes structure where
                            we will allocate space and store the UEFI memory map

  @retval EFI_SUCCESS       UEFI memory map was populated successfully
  @retval other             There was a failure retrieving or populating the UEFI mememory map

**/
static EFI_STATUS GetUefiMemoryMap(SMC_PARAMETERS_MAP_MEMORY *memmap)
{
  UINTN MemoryMapSize = 0;
  UINTN MapKey = 0;
  UINTN DescriptorSize = 0;
  UINT32 DescriptorVersion = 0;
  EFI_STATUS status = EFI_SUCCESS;

  // Call the first time to determine the size
  status = gBS->GetMemoryMap ( &MemoryMapSize, NULL, &MapKey,
                      &DescriptorSize, &DescriptorVersion);

  if (status == EFI_BUFFER_TOO_SMALL) {
    status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                        EFI_SIZE_TO_PAGES(MemoryMapSize), (VOID*)&memmap->MemoryMappings);
    if(status != EFI_SUCCESS || memmap->MemoryMappings == NULL ) {
      AsciiPrint("Failed to allocate memory for UEFI Memory Map\n");
      if(status == EFI_SUCCESS) status = EFI_OUT_OF_RESOURCES;
      return status;
    }

    // Call again to get the actual UEFI memory map
    status = gBS->GetMemoryMap (&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)memmap->MemoryMappings,
                    &MapKey, &DescriptorSize, &DescriptorVersion);
    if( status == EFI_SUCCESS) {
      // List size is the number of entries in the list
      memmap->MemoryMappingCount = MemoryMapSize / DescriptorSize;
      memmap->MemoryMappingVersion = DescriptorVersion;
      memmap->MemoryMappingSize = DescriptorSize;
      AsciiPrint("Successfully read UEFI Memory Map address: 0x%x size: 0x%x\n",
              memmap->MemoryMappings, memmap->MemoryMappingCount);
    } else {
      AsciiPrint("GetMemoryMap Failed to retrieve UEFI Memory Map\n");
      // Free memory we have allocated
      gBS->FreePages((EFI_PHYSICAL_ADDRESS)memmap->MemoryMappings,
                      EFI_SIZE_TO_PAGES(MemoryMapSize));
      memmap->MemoryMappings = NULL;
    }
  }
  return status;
}

EFI_STATUS
RebaseImageRead (
  IN     VOID    *FileHandle,
  IN     UINTN   FileOffset,
  IN OUT UINT32  *ReadSize,
  OUT    VOID    *Buffer
  )
/*++

Routine Description:

  Support routine for the PE/COFF Loader that reads a buffer from a PE/COFF file

Arguments:

  FileHandle - The handle to the PE/COFF file

  FileOffset - The offset, in bytes, into the file to read

  ReadSize   - The number of bytes to read from the file starting at FileOffset

  Buffer     - A pointer to the buffer to read the data into.

Returns:

  EFI_SUCCESS - ReadSize bytes of data were read into Buffer from the PE/COFF file starting at FileOffset

--*/
{
  CHAR8   *Destination8;
  CHAR8   *Source8;
  UINT32  Length;

  Destination8  = Buffer;
  Source8       = (CHAR8 *) ((UINTN) FileHandle + FileOffset);
  Length        = *ReadSize;
  while (Length--) {
    *(Destination8++) = *(Source8++);
  }

  return EFI_SUCCESS;
}

static EFI_STATUS ReloatePECoffImage(UINT64 skFileBuffer, PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  EFI_IMAGE_OPTIONAL_HEADER_UNION       *ImgHdr;
  EFI_STATUS                            status = EFI_SUCCESS;

  memset (ImageContext, 0, sizeof (PE_COFF_LOADER_IMAGE_CONTEXT));
  ImageContext->Handle     = (VOID *) skFileBuffer;
  ImageContext->ImageRead  = (PE_COFF_LOADER_READ_FILE) RebaseImageRead;
	
	status = PeCoffLoaderGetImageInfo (ImageContext);
	if(EFI_ERROR(status))
	{
		AsciiPrint("Error in PeCoffLoaderGetImageInfo:0x%x ",status);
		return EFI_ABORTED;
	}
	 if (ImageContext->RelocationsStripped) {
          AsciiPrint("The input PeImage %s has no relocation to be fixed up");
    }

  //
  // Get PeHeader pointer
  //
  ImgHdr = (EFI_IMAGE_OPTIONAL_HEADER_UNION *)((VOID *)skFileBuffer + ImageContext->PeCoffHeaderOffset);

  //
  // Load and Relocate Image Data
  //
  AsciiPrint("ImageContext.ImageSize =%d, ImageContext.SectionAlignment=%d",ImageContext->ImageSize,ImageContext->SectionAlignment);
  status=gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      (UINTN) ImageContext->ImageSize + ImageContext->SectionAlignment, (VOID*)&LauncherImagePtr);
  if (EFI_ERROR(status)) {
    AsciiPrint("memory cannot be allocated on rebase");
    return EFI_OUT_OF_RESOURCES;
  }
  memset ((VOID *) LauncherImagePtr, 0, (UINTN) ImageContext->ImageSize + ImageContext->SectionAlignment);
  ImageContext->ImageAddress = ((UINTN) LauncherImagePtr + ImageContext->SectionAlignment - 1) & (~((INT64)ImageContext->SectionAlignment - 1));

  status =  PeCoffLoaderLoadImage (ImageContext);
  if (EFI_ERROR (status)) {
    AsciiPrint("LoadImage() call failed on rebase");
  }

  return status;
}

static EFI_STATUS LoadPECoffImageFromDisk(CHAR16 *name, PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  UINTN nHandles,i;
  EFI_STATUS status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Fs=NULL;
  EFI_FILE_PROTOCOL *FsVolume = NULL;
  EFI_HANDLE *HandleBuffer = NULL;
  EFI_FILE_PROTOCOL *FilePtr = NULL;
  UINT64 skFileBuffer = 0x0;
  UINT64 buffersize = SK_BUFFER_SIZE;
  
  status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid,
                                  NULL, &nHandles, &HandleBuffer);
  if(status == EFI_SUCCESS) 
  {
    for (i = 0; i < nHandles; i++) {
      //loop through all handles, open volume and check if file is there
      status = gBS->HandleProtocol(HandleBuffer[i],&gEfiSimpleFileSystemProtocolGuid,(VOID **)&Fs);
      AsciiPrint(" handle cnt %d, total: %d\n", i,nHandles);

      if(status != EFI_SUCCESS) {
        AsciiPrint("ERROR: Failed to get FS handles for simple FS protocol.\n");
        continue;
      }

      status = Fs->OpenVolume (Fs, &FsVolume);

      if (status != EFI_SUCCESS) {
        AsciiPrint("ERROR: Failed to open FS volume\n");
        continue;
      }

      //Check if file is present
      status = FsVolume->Open(FsVolume, &FilePtr, name, EFI_FILE_MODE_READ, 0);

      if ((status != EFI_SUCCESS) || (FilePtr == NULL)) {
        if (i ==nHandles -1) {
          AsciiPrint("%s not found in all the fat partitions",name);
          status = EFI_ABORTED;
          break;
        }
        continue;
      }
    
      AsciiPrint("%s found\n",name);
      break;
    }
  } else {
    AsciiPrint("ERROR: Failed to get handle buffer for simple FS protocol.\n");
  }

  for( ;(status == EFI_SUCCESS) && (FilePtr != NULL);) {
    status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                        EFI_SIZE_TO_PAGES(buffersize), (VOID*)&skFileBuffer);
                        
    if (EFI_ERROR(status))
    {
      AsciiPrint("Error allocating buffer to load %s (size:0x%x)", name, buffersize);
      break;
    }
    
    AsciiPrint("Allocated buffer to read %s into memory(size:0x%x) Location:0x%x.\n", name, buffersize, (VOID *)skFileBuffer);
    
    status=FilePtr->Read(FilePtr,&buffersize,(VOID *)skFileBuffer);
    if(EFI_ERROR(status))
    {
      AsciiPrint("Error reading %s into memory buffer.",name);
      break;
    }
    
    AsciiPrint("completed reading %s into memory buffer (bytes read:0x%x).\n", name, buffersize);
    FilePtr->Close(FilePtr);
    
    // Now that image has been loaded into UEFI memory, relocate it to final location
    status = ReloatePECoffImage(skFileBuffer,ImageContext);
    if( status != EFI_SUCCESS ) {
        AsciiPrint("ReloatePECoffImage   [FAILED]\n");
    }
    
    // If we make it here we are successful so break;
    break;
  }

  // Now that image has been relocated we can free our temp UEFI buffer
  if( skFileBuffer != 0x0) {
    gBS->FreePages((EFI_PHYSICAL_ADDRESS)skFileBuffer,EFI_SIZE_TO_PAGES(buffersize));
    skFileBuffer = 0x0;
  }
  
  return status;
}

static EFI_STATUS SKGetVersion(PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  EFI_STATUS status       = EFI_SUCCESS;
  SMC_PARAMETERS *version = NULL;
  
  status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS)), (VOID*)&version);
					  
  if (EFI_SUCCESS != status || version == NULL) {
      AsciiPrint("Allocate memory of version attributes   [FAILED]\n");
      return status;
  }
  
  version->Header.Size   = sizeof(SMC_TABLE_HEADER);
  version->Header.Action = HYP_SECURE_KERNEL_VERSION;
  version->Header.Reserved = 0;

  // Send the command to authenticate
  WriteBackInvalidateDataCacheRange (version,
         ROUND_TO_PAGE(sizeof(SMC_PARAMETERS)));
  status = HypSKLaunch(version, HYP_SECURE_KERNEL_VERSION);
  
  // Validate major version and minor version returned from Hyp.
  if((version->Header.MajorVersion != OEM_EL2_LAUNCH_MAJOR_VERSION) ||
  (version->Header.MinorVersion < OEM_EL2_LAUNCH_MINOR_VERSION))
  {
	  AsciiPrint("Get major version, minor version failed   [FAILED]\nExpected values:\nMajor version --> 0x%x\nMinor version --> 0x%x\n", OEM_EL2_LAUNCH_MAJOR_VERSION, OEM_EL2_LAUNCH_MINOR_VERSION);
	  status = EFI_NOT_FOUND;
  }
  
  else
  {
	  AsciiPrint("Get major version, minor passed   [PASSED]\nMajor version --> 0x%x\nMinor version --> 0x%x\n", version->Header.MajorVersion, version->Header.MinorVersion);
  }  
  
  // This structure should be returned back to us so free it
  gBS->FreePages((EFI_PHYSICAL_ADDRESS)version,EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS)));
  
  // update this to also return major, minor versions
  return status;
					  
}


static EFI_STATUS SKLaunchImage(PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  EFI_STATUS status = EFI_SUCCESS;
  SMC_PARAMETERS_LAUNCH *launch = NULL;

  status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_LAUNCH)), (VOID*)&launch);

  if (EFI_SUCCESS != status || launch == NULL) {
      AsciiPrint("Allocate memory of launch attributes   [FAILED]\n");
      return status;
  }

  AsciiPrint("LauncherImagePtr:0x%x ImageSize:0x%x\n",(UINT64)LauncherImagePtr,ImageContext->ImageSize);
  
  /* header Todo, update the required Revision and Size */
  launch->Header.MajorVersion = OEM_EL2_LAUNCH_MAJOR_VERSION;
  launch->Header.MinorVersion = OEM_EL2_LAUNCH_MINOR_VERSION;
  launch->Header.Size = sizeof(SMC_TABLE_HEADER);
  launch->Header.Action = HYP_SECURE_KERNEL_CANCEL;
  launch->Header.Reserved = 0;
  
  launch->SecLaunchImageBase = (UINT64)ImageContext->ImageAddress;
  launch->SecLaunchImageSize = ImageContext->ImageSize;
  launch->SecLaunchDataBase = LauncherDataPtr;
  launch->SecLaunchDataSize = 0x1000;
  launch->SecLaunchImageEntry = (UINT64)ImageContext->EntryPoint;
  
  // Send the command to authenticate
  WriteBackInvalidateDataCacheRange (launch,
         ROUND_TO_PAGE(sizeof(SMC_PARAMETERS_LAUNCH)));
  status = HypSKLaunch((SMC_PARAMETERS *)launch, HYP_SECURE_KERNEL_LAUNCH);
  
  // On successfully launch QHEE never gets a chance to free this for us
  //gBS->FreePages((EFI_PHYSICAL_ADDRESS)launch,EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_LAUNCH)));
  
  return status;
}

static EFI_STATUS SKCancelLoad(void)
{
  // Allocate memory and make authenticate call
  EFI_STATUS status = EFI_SUCCESS;
  SMC_PARAMETERS_CANCEL *cancel = NULL;

  status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_CANCEL)), (VOID*)&cancel);

  if (EFI_SUCCESS != status || cancel == NULL) {
      AsciiPrint("Allocate memory of launch attributes   [FAILED]\n");
      return status;
  }

    /* header Todo, update the required Revision and Size */
  cancel->Header.MajorVersion = OEM_EL2_LAUNCH_MAJOR_VERSION;
  cancel->Header.MinorVersion = OEM_EL2_LAUNCH_MINOR_VERSION;
  cancel->Header.Size = sizeof(SMC_TABLE_HEADER);
  cancel->Header.Action = HYP_SECURE_KERNEL_CANCEL;
  cancel->Header.Reserved = 0;
  
  // Send the command to authenticate
  WriteBackInvalidateDataCacheRange (cancel,
         ROUND_TO_PAGE(sizeof(SMC_PARAMETERS_CANCEL)));
  status = HypSKLaunch((SMC_PARAMETERS *)cancel, HYP_SECURE_KERNEL_CANCEL);
  
  // This structure should be returned back to us so free it
  gBS->FreePages((EFI_PHYSICAL_ADDRESS)cancel,EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_CANCEL)));
  
  return status;
}

static EFI_STATUS SKAuthenticateImage(PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  // Allocate memory and make authenticate call
  EFI_STATUS status = EFI_SUCCESS;
  SMC_PARAMETERS_AUTHENTICATE *auth = NULL;

  status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_AUTHENTICATE)), (VOID*)&auth);

  if (EFI_SUCCESS != status || auth == NULL) {
      AsciiPrint("Allocate memory of launch attributes   [FAILED]\n");
      return status;
  }

    /* header Todo, update the required Revision and Size */
  auth->Header.MajorVersion = OEM_EL2_LAUNCH_MAJOR_VERSION;
  auth->Header.MinorVersion = OEM_EL2_LAUNCH_MINOR_VERSION;
  auth->Header.Size = sizeof(SMC_TABLE_HEADER);
  auth->Header.Action = HYP_SECURE_KERNEL_AUTHENTICATE;
  auth->Header.Reserved = 0;

  // Note the first 4K is just headers and no data is in there
  // MSFT will need to pass in the image before it has been remapped so this
  // will not be an issue for now pass in a mapped address so it will pass our checks
  auth->SecLaunchImageBase = (UINT64)ImageContext->ImageAddress;
  auth->SecLaunchImageSize = ImageContext->ImageSize;
  auth->TzAppDataBase = LauncherDataPtr; // For now don't pass in any data to TZ
  auth->TzAppDataSize =0x1000;
  
  // Send the command to authenticate
  WriteBackInvalidateDataCacheRange (auth,
         ROUND_TO_PAGE(sizeof(SMC_PARAMETERS_AUTHENTICATE)));
  status = HypSKLaunch((SMC_PARAMETERS *)auth, HYP_SECURE_KERNEL_AUTHENTICATE);
  
  // This structure should be returned back to us so free it
  gBS->FreePages((EFI_PHYSICAL_ADDRESS)auth,EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_AUTHENTICATE)));
  
  return status;
}

/**
  Allocate pages for all the parameters passed into HYP and initialize data
  buffers with some known data so we can validate the data from HYP.

  @param[in] attributePtr   A pointer to memory that has been allocated to store
                            all the launch parameters information

  @retval EFI_SUCCESS       All structures have been successfully initialized
  @retval other             There was a failure during initializing structures

**/
static EFI_STATUS SKAllocateAndMapMemory(PE_COFF_LOADER_IMAGE_CONTEXT *ImageContext)
{
  const UINT32 EL2_MAPPING_COUNT = 3;
  EFI_STATUS status =  EFI_SUCCESS;
  SMC_MEMORY_MAPPING *El2MemoryPtr = NULL;
  SMC_PARAMETERS_MAP_MEMORY *mapmem = NULL;

  /* Allocate memory for Launch attributes*/
  status=gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_PARAMETERS_MAP_MEMORY)), (VOID*)&mapmem);

  if (EFI_SUCCESS != status || mapmem == NULL)
  {
      AsciiPrint("Allocate memory of launch attributes   [FAILED]\n");
      goto exitSKAllocateAndMapMemory;
  }
  
  /* Allocate memory for EL2MemoryMappings */
  status=gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(sizeof(SMC_MEMORY_MAPPING)*EL2_MAPPING_COUNT),
                      (VOID*)&El2MemoryPtr);
                      
  if (EFI_SUCCESS != status || El2MemoryPtr == NULL)
  {
      AsciiPrint("Allocate memory of launch image   [FAILED]\n");
      if(status == EFI_SUCCESS) status = EFI_OUT_OF_RESOURCES;
      goto exitSKAllocateAndMapMemory;
  }

  /* Allocate memory for Launcher Data, Assuming data size is 4KB */
  status=gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData,
                      EFI_SIZE_TO_PAGES(0x1000), (VOID*)&LauncherDataPtr);
  if (EFI_SUCCESS != status || LauncherDataPtr == 0x0)
  {
      AsciiPrint("Allocate memory of Launcher Data   [FAILED]\n");
      if(status == EFI_SUCCESS) status = EFI_OUT_OF_RESOURCES;
      goto exitSKAllocateAndMapMemory;
  }

  // Put some data in the LauncherDataPtr
  UINT32 *launchData = (UINT32 *)LauncherDataPtr;
  for(int i=0; i < 0x400; i++) {
    *launchData++ = 0x0; // Set data to 0 so it will return without trying to actually parse this.
  }

  // Create a map memory request

  // header Todo, update the required Revision and Size
  mapmem->Header.MajorVersion = OEM_EL2_LAUNCH_MAJOR_VERSION;
  mapmem->Header.MinorVersion = OEM_EL2_LAUNCH_MINOR_VERSION;
  mapmem->Header.Size = sizeof(SMC_TABLE_HEADER);
  mapmem->Header.Action = HYP_SECURE_KERNEL_MAP_MEMORY;
  mapmem->Header.Reserved = 0;

  mapmem->MemoryMappingCount = EL2_MAPPING_COUNT;
  mapmem->MemoryMappingSize = sizeof(SMC_MEMORY_MAPPING);
  mapmem->MemoryMappingVersion = 0x2;
  mapmem->MemoryMappings = El2MemoryPtr;

  // Map in entire image space as RW
  El2MemoryPtr[0].VirtualAddress = (UINT64)ImageContext->ImageAddress;
  El2MemoryPtr[0].PhysicalAddress = (UINT64)ImageContext->ImageAddress;
  El2MemoryPtr[0].Size = ImageContext->ImageSize;
  El2MemoryPtr[0].PermAttr = VM_PERM_RW;
  El2MemoryPtr[0].CacheAttr = VM_CACHE_OUTER_NC_INNER_NC;
  El2MemoryPtr[0].Flags = 0x0;

  El2MemoryPtr[1].VirtualAddress = (UINT64)LauncherDataPtr;
  El2MemoryPtr[1].PhysicalAddress = (UINT64)LauncherDataPtr;
  El2MemoryPtr[1].Size = 0x1000;
  El2MemoryPtr[1].PermAttr = VM_PERM_RW;
  El2MemoryPtr[1].CacheAttr = VM_CACHE_OUTER_NC_INNER_NC;
  El2MemoryPtr[1].Flags = 0x0;
  
  // Map entire HLOS memory as DMA so it doesn't get unmapped
  El2MemoryPtr[2].VirtualAddress = 0x80000000;
  El2MemoryPtr[2].PhysicalAddress = 0x80000000;
  El2MemoryPtr[2].Size = 0x200000000;
  El2MemoryPtr[2].PermAttr = VM_PERM_RW;
  El2MemoryPtr[2].CacheAttr = VM_CACHE_OUTER_NC_INNER_NC;
  El2MemoryPtr[2].Flags = SMC_MEMORY_MAPPING_FLAG_DMA;

  /* Setup UEFI memory descriptor */
  //status = GetUefiMemoryMap(attributePtr);

  // Flush all the caches before sending data across to HYP
  WriteBackInvalidateDataCacheRange (mapmem,
         ROUND_TO_PAGE(sizeof(SMC_PARAMETERS_MAP_MEMORY)));
  WriteBackInvalidateDataCacheRange (mapmem->MemoryMappings,
         ROUND_TO_PAGE(sizeof(SMC_MEMORY_MAPPING)*mapmem->MemoryMappingCount));

  // Flush the cache for all data regions we have mapped that will be passed to HVX
  for(int i=0; i < mapmem->MemoryMappingCount; i++) {
    // Only invalidate if it is normal memory
    if(mapmem->MemoryMappings[i].Flags == 0 ) {
      WriteBackInvalidateDataCacheRange ((void*)mapmem->MemoryMappings[i].VirtualAddress,
          ROUND_TO_PAGE(mapmem->MemoryMappings[i].Size));
    }
  }
  
  // Map all the memory regions into EL2
  status = HypSKLaunch((SMC_PARAMETERS *)mapmem, HYP_SECURE_KERNEL_MAP_MEMORY);

exitSKAllocateAndMapMemory:

  // Always free the EL2 memory after mapping has been completed shouldn't be used anymore
  if(El2MemoryPtr != NULL) {
    gBS->FreePages((EFI_PHYSICAL_ADDRESS)El2MemoryPtr,EFI_SIZE_TO_PAGES(sizeof(VMMapAttr_t)*EL2_MAPPING_COUNT));
    El2MemoryPtr = NULL;
  }

  // Only free this structure if we failed to map the regions
  if(status != EFI_SUCCESS) {
    if(LauncherDataPtr != 0x0) {
      gBS->FreePages((EFI_PHYSICAL_ADDRESS)LauncherDataPtr,EFI_SIZE_TO_PAGES(0x1000));
      LauncherDataPtr = 0x0;
    }
    
  }

  return status;
}

/**
  Test SK Launch SMC calls to HYP to launch and run test HVX code

  @retval EFI_SUCCESS       All test cases passed successfully
  @retval other             Error occurred when trying to launch image

**/
EFI_STATUS
EFIAPI TestSKLaunchCall( void )
{
  EFI_STATUS status =  EFI_SUCCESS;
  PE_COFF_LOADER_IMAGE_CONTEXT ImageContext;
  
  // Load sk.exe from disk 
  status = LoadPECoffImageFromDisk(L"SK.exe", &ImageContext);
  if( status != EFI_SUCCESS ) {
      AsciiPrint("LoadPECoffImageFromDisk   [FAILED]\n");
      goto exitTestSKLaunchCall;
  }
  
  // Allocate memory from UEFI for all the structures we need and map memory regions into EL2  
  
  status = SKGetVersion(&ImageContext);
  if( status != EFI_SUCCESS ) {
    AsciiPrint("SKGetVersion   [FAILED]\n");
    goto exitTestSKLaunchCall;
  }
  
  status = SKAllocateAndMapMemory(&ImageContext);
  if( status != EFI_SUCCESS ) {
    AsciiPrint("SKAllocateAndMapMemory   [FAILED]\n");
    goto exitTestSKLaunchCall;
  }

  status = SKAuthenticateImage(&ImageContext);
  if( status != EFI_SUCCESS ) {
    AsciiPrint("SKAuthenticateImage   [FAILED]\n");
    goto exitTestSKLaunchCall;
  }

  status = SKLaunchImage(&ImageContext);
  if( status != EFI_SUCCESS ) {
    AsciiPrint("SKLaunchImage   [FAILED]\n");
    goto exitTestSKLaunchCall;
  }

exitTestSKLaunchCall:
  // These memory regions were permanently given to QHEE after launch
  //FreeSKLaunchMemory(attributePtr);
  return status;
}

EFI_STATUS test_hyp_call(void)
{
  ArmSMCCall(TZ_PSCI_MEM_PROTECT, 1, 0);
  
  return EFI_SUCCESS;
}

/**
  An empty function to pass error checking of CreateEventEx ().

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.

**/
void EFIAPI ACPITableLoadEmptyFuntion (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}


static EFI_STATUS LoadACPITables(void)
{
  EFI_STATUS status;

  // Signal ACPI Table Loading
  status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               ACPITableLoadEmptyFuntion,
                               NULL,
                               &gEfiACPITableLoadGuid,
                               &ACPITableLoadEvent);
  if (status == EFI_SUCCESS)
    gBS->SignalEvent (ACPITableLoadEvent);

  return status;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS status = EFI_SUCCESS;

  TEST_START("HypervisorTest");
/*
  status = test_hyp_call();
  if (EFI_ERROR (status)) {
      AsciiPrint("test_hyp_call failed, Status = (0x%x)\r\n", status);
   } else {
      AsciiPrint("test_hyp_call test passed\n");
   }
*/
  status = LoadACPITables();
  if (EFI_ERROR (status)) {
    AsciiPrint("LoadACPITables failed, Status = (0x%x)\r\n", status);
  }

  gHvxTestHandle=ImageHandle;
  TEST_START("Secure kernel launch test");
  status = TestSKLaunchCall();
  
  if (EFI_ERROR (status)) {
    AsciiPrint("TestSKLaunchCall failed, Status = (0x%x)\r\n", status);

  } else {
    AsciiPrint("TestSKLaunchCall passed.\n");
  }
  
  return status;
}
