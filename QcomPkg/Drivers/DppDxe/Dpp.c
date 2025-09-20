/** @file Dpp.c

  Implementation for Qualcomm UEFI DPP protocol

  Copyright (c) 2012-2018 Qualcomm Technologies, Inc. All rights 
  reserved. 

**/

/*=============================================================================
                              EDIT HISTORY


when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
07/17/18   gparam  Cleaned up function description
05/31/18   prema   Fixed issues related to read and write files to raw file system
04/12/18   ltg     Added raw file system calls for accessing block based DPP
04/16/15   mic     Updated to support 64 bit arch 
09/05/13   vk      Warning cleanup
07/16/13   mic     Added FAT-based DPP protocol support
11/05/12   rs      Added support for deletion of DPP items 
03/25/12   jd      Initial revision
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Library/BootConfig.h>

#include <Guid/GlobalVariable.h>
#include <Guid/FileInfo.h>
#include <Guid/FileSystemInfo.h>

#include <Pi/PiDxeCis.h>

#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/EFIRawFileSystem.h>

/**
  DPP PROTOCOL interfaces
 */
#include <Protocol/EFIDpp.h>

#include "Dpp.h"

#define ERR_HANDLE(Status)\
  do { \
    if (EFI_ERROR (Status)) { \
      return Status; \
    } \
  } while (0)

#define HANDLE_ERROR_LABEL(Status, Function, Label) \
  do {  \
    if(EFI_ERROR(Status))   \
    {   \
      DEBUG((EFI_D_ERROR,": %a returned %r\n", #Function, Status)); \
      goto Label; \
    }   \
  }while (0)

#define HANDLE_ERR_CONTINUE(Status, Function) \
  do {  \
    if(EFI_ERROR(Status))   \
    {   \
      DEBUG((EFI_D_ERROR,": %a returned %r...Continue\n", #Function, Status)); \
    }   \
  }while (0)

#define DEBUG_ERR_LABEL(MESSAGE, Label) \
  do {  \
       DEBUG((EFI_D_ERROR, MESSAGE)); \
       goto Label; \
  }while (0)


#define ALIGN_TO_BLOCK(Size, BlockSize, AlignedSize) \
  { \
    AlignedSize = Size + (BlockSize - 1); \
    AlignedSize = AlignedSize & ~(BlockSize -1); \
  }

#define TYPE_MEDIA_DEVICE_PATH 4
#define DEFAULT_FS_BLOCKSIZE 256    // 1/8 of TZ Block Size

static DPP_PARTITION_TYPE            gDppPartType   = DPP_PARTITION_TYPE_UNKNOWN;
static EFI_RAW_FILE_SYSTEM_PROTOCOL *gRawFSProtocol = NULL;
static EFI_GUID                     *gDiskTypeGuid  = NULL;
static EFI_HANDLE                    gDPPFatHandle  = NULL;

static EFI_GUID gEfiDppBlockPartitionGuid = 
  {0xF97B8793, 0x3ABF, 0x4719,{ 0x89, 0x6B, 0x7C, 0x3E, 0x9B, 0x85, 0xE1, 0x04}};

static EFI_GUID gEfiDppFatPartitionGuid = 
  {0xebd0a0a2, 0xb9e5, 0x4433, {0x87, 0xc0, 0x68, 0xb6, 0xb7, 0x26, 0x99, 0xc7}};

extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gEfiSpiNor0Guid;

/**
 * Validate and correct FileInfo time
 * 
 * @param Time   - FAT file info structure
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
ValidateAndCorrectFileInfoTime(
  IN OUT EFI_TIME *Time
  )
{

  if (Time == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  /*if any bad variable found in Time structure, set the whole
    time structure to default value 
  */
  if (Time->Year  < 1980 ||
      Time->Month < 1 ||
      Time->Month > 12 ||
      Time->Day   < 1 ||
      Time->Day   > 31 ||
      Time->Hour  > 23 ||
      Time->Minute > 59 ||
      Time->Second > 59 ||
      Time->Nanosecond > 999999999
      ) 
  {
    Time->Year = 2012;
    Time->Month = 1;
    Time->Day = 1;
    Time->Hour = 1;
    Time->Minute = 1;
    Time->Second = 1;
    Time->Nanosecond = 1;
  
  }

  return EFI_SUCCESS;
}


/**
 * Reads the FileInfo from a given file.  Caller needs to
 * free the allocated FileInfo.
 * 
 * @param File         - Handle to File
 * @param FileInfoSize - Output to file info size
 * @param FileInfo     - Returns modified pointer to FileInfo
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
GetFatFileInfo(
    IN EFI_FILE_PROTOCOL  *File,
    IN OUT UINTN          *FileInfoSize,
    IN OUT EFI_FILE_INFO **FileInfo
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
    
  if ((File == NULL) || (FileInfoSize == NULL) || (FileInfo == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *FileInfoSize = 0;
  *FileInfo = NULL;

  /* Get the required FileInfo buffer size by giving *FileInfoSize = 0,
     Status is expected to be EFI_BUFFER_TOO_SMALL*/
  Status = File->GetInfo(File,
                         &gEfiFileInfoGuid,
                         FileInfoSize,
                         (VOID*)*FileInfo);

  if(Status == EFI_BUFFER_TOO_SMALL)
  {    
    Status = gBS->AllocatePool(EfiBootServicesData,
                               *FileInfoSize,
                               (VOID**)FileInfo);
    HANDLE_ERROR_LABEL (Status, gBS->AllocatePool, GetFatFileInfoExit);

    Status = File->GetInfo(File,
                           &gEfiFileInfoGuid,
                           FileInfoSize,
                           (VOID*)*FileInfo);
    HANDLE_ERROR_LABEL (Status, File->GetInfo, GetFatFileInfoExit);
  }
  else if( EFI_ERROR(Status))
  {
    HANDLE_ERROR_LABEL (Status, File->GetInfo, GetFatFileInfoExit);
  }
  else
  {
    /* This is to handle EFI_SUCCESS case, status shouldn't be EFI_SUCCESS if
       *FileInfoSize is 0.*/
    Status = EFI_UNSUPPORTED;
    HANDLE_ERROR_LABEL (Status, File->GetInfo, GetFatFileInfoExit);
  }
  
  return Status;

GetFatFileInfoExit:

  if(*FileInfo != NULL)
  {
    gBS->FreePool(*FileInfo);
    *FileInfo = NULL;
  }
  return Status;
}

/**
 * Creates a directory in the given File system root.
 *  
 * @param FsRoot      - File System Root where directory will be
 *                      created.
 * @param FileName    - Full path of folder to create.
 * @param Attributes  - Attributes passed in
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
CreateFatDirectory(
    IN EFI_FILE_PROTOCOL    *FsRoot,
    IN UINT16               *FileName,
    IN UINT64               Attributes)
{
  EFI_STATUS            Status        = EFI_SUCCESS;
  EFI_FILE_PROTOCOL    *File          = NULL;
  UINT64                OpenMode      = 0;
  UINT16              **Paths         = NULL; 
  UINT16              **NewPaths      = NULL;
  UINT16               *CurrentPath   = NULL;
  UINTN                 CharCounter   = 0;
  UINTN                 StringCounter = 0;
  UINTN                 StringCount   = 0;
    
  //Find how many directories we need to create.
  for(CharCounter = 0; CharCounter < StrLen(FileName); CharCounter++)
  {
    if((FileName[CharCounter] == L'\\') && (CharCounter > 0) ) 
    {
       //Make new string for this directory
       Status = gBS->AllocatePool(EfiBootServicesData,
                                  CharCounter<<1,
                                  (VOID**)&CurrentPath);
       ERR_HANDLE(Status);

       CopyMem(CurrentPath,FileName,CharCounter<<1);
       CurrentPath[CharCounter] = L'\0';

       //Add this directory string to the list of directories

       Status = gBS->AllocatePool(EfiBootServicesData,
                                  StringCount+1,
                                  (VOID**)&NewPaths);
       ERR_HANDLE(Status);

       if(StringCount > 0)
       {
         //Copy current paths to the expanded paths list
         CopyMem(NewPaths,Paths,(StringCount*sizeof(UINT16*)) );

         //Cleanup old Paths List
         Status = gBS->FreePool((VOID*)Paths);
         ERR_HANDLE(Status);    
       }
       Paths = NewPaths;
       Paths[StringCount] = CurrentPath;
       StringCount++;
    }
  }
  OpenMode = EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE;
  Attributes |= EFI_FILE_DIRECTORY;
  //Create Each directory, but do not create the file.
  for(StringCounter = 0; StringCounter < StringCount; StringCounter++)
  {    
    Status = FsRoot->Open(FsRoot,
                      &File,
                      Paths[StringCounter],                          
                      OpenMode,
                      Attributes);
    ERR_HANDLE(Status);
  }
  return Status;
}

/**
 * Function to get a matching partition handle
 * after comparing the Emmc type, Partition Name and 
 * Partition type GUID
 *  
 * @param PartHandle    - partition handle
 * @Param PartTypeGuid  - Partition type GUID
 * @Param PartitionName - Partition name
 * @Param Protocol      - Protocol
 * @return : EFI_STATUS
 */
EFI_STATUS
GetPartitionHandle(
    IN OUT EFI_HANDLE              *PartHandle,
    IN EFI_GUID                    *PartTypeGuid,
    IN CHAR16                      *PartitionName,
    IN EFI_GUID                    *Protocol
    )
{
  EFI_STATUS                 Status             = EFI_SUCCESS;
  EFI_HANDLE                *HandleBuffer       = NULL;
  UINTN                      NumHandles         = 0;
  UINTN                      HandleCounter      = 0;
  EFI_GUID                  *TempPartTypeGuid   = NULL;
  CHAR16                    *TempPartitionName  = NULL;
  EFI_PARTITION_ENTRY       *TempPartEntry      = NULL;
  EFI_DEVICE_PATH_PROTOCOL  *DevPathInst        = NULL;

  //allow PartitionName to be NULL and match only GUID
  if ((PartHandle == NULL) ||
      (PartTypeGuid == NULL) ||
      (Protocol == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }
     
  Status = gBS->LocateHandleBuffer(ByProtocol,
                                   Protocol,
                                   NULL,
                                   &NumHandles,
                                   &HandleBuffer);
  HANDLE_ERROR_LABEL(Status,gBS->LocateHandleBuffer,GetPartitionHandle);
         
  for(HandleCounter = 0; HandleCounter < NumHandles; HandleCounter++)
  {   
    Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                                &gEfiPartitionTypeGuid,
                                (VOID**)&TempPartTypeGuid);    
    if(EFI_ERROR(Status))
    {
      continue;
    }

    Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                                 &gEfiDevicePathProtocolGuid,
                                (VOID**)&DevPathInst); 
    if(EFI_ERROR(Status))
    {
      continue;
    }

    Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                               &gEfiPartitionRecordGuid,
                               (VOID **) &TempPartEntry);
    if(EFI_ERROR(Status))
    {
      continue;
    }

    TempPartitionName = TempPartEntry->PartitionName;
    DevPathInst = (EFI_DEVICE_PATH_PROTOCOL*)(((UINT8*)DevPathInst)+DevPathInst->Length[0]);

    if(DevPathInst->Type == TYPE_MEDIA_DEVICE_PATH)
    {    
      if(CompareGuid(TempPartTypeGuid,PartTypeGuid)) 
      {
		if((PartitionName==NULL) || (PartitionName!=NULL && CompareMem(TempPartitionName, PartitionName, StrSize(TempPartitionName)) == 0))
		{
		   *PartHandle = HandleBuffer[HandleCounter];
		    Status = EFI_SUCCESS;
			goto GetPartitionHandle;
		}
      }
    }                   
  }
  
  Status = EFI_NOT_FOUND;

GetPartitionHandle:
  if(HandleBuffer != NULL)
  {
    gBS->FreePool(HandleBuffer);
    HandleBuffer = NULL;
  }
  
  return Status;
};


/**

  BlockSetDPP()

  Stores device provision data into block-based DPP.

   
  @param[in]  Type          An unicode string specifying provision data type,
                            e.g. "QCOM" or "OEM".
  @param[in]  Name          An unicode string specifying provision data name,
                            e.g. "WLAN.PROVISION"
  @param[in]  Data          A pointer to the buffer that contains provision data
                            to be written.
  @param[in]  DataSize      The size of provision data, in bytes.

  @return
  EFI_SUCCESS               Provision data is written to DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter.
  EFI_OUT_OF_RESOURCES      No enough system resource 
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)

**/
EFI_STATUS
BlockSetDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN VOID               *Data,
  IN UINTN               DataSize
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  CHAR16                 *pFilePath = NULL;
  UINTN                   nTypeLen = 0;
  UINTN                   nNameLen = 0;

  // Check input parameters
  if ((Type == NULL) ||
      (Name == NULL) ||
      (Data == NULL) ||
      (DataSize == 0))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockSetDPPExit;
  }
  
    nTypeLen = StrLen(Type);
    nNameLen = StrLen(Name);

  if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
      (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockSetDPPExit;
  }

  // construct file name
  pFilePath = AllocateZeroPool(sizeof(CHAR16) * (nTypeLen + nNameLen + StrSize(L"\\")));
  if (pFilePath != NULL)
  {
    StrCat(pFilePath, Type);
    StrCat(pFilePath, L"\\");
    StrCat(pFilePath, Name);
  }
  else
  {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((EFI_D_ERROR, "[DppDxe] Could not allocate memory for file name. Status = 0x%x.\n", Status));
    goto BlockSetDPPExit;
  }

  // write file
  Status = gRawFSProtocol->FileWrite(gRawFSProtocol, Data, DataSize, pFilePath, NULL, &gEfiDppBlockPartitionGuid, gDiskTypeGuid);

    // If Raw FS protocol could not write file, print an error message and return
  if(EFI_ERROR(Status))
  {
    if (Status == EFI_VOLUME_CORRUPTED)
    {
      DEBUG((EFI_D_ERROR, "[DppDxe] DPP partition header is corrupted! Partition needs to be re-formatted.\n"));
      goto BlockSetDPPExit;
    }
    else
    {
      DEBUG((EFI_D_ERROR, "[DppDxe] Failed to write DPP block data with RawFSProtocol.\n"));
    }
  }

BlockSetDPPExit:

    if (pFilePath != NULL)
    {
        FreePool(pFilePath);
        pFilePath = NULL;
    }

  return Status;
}


/**

  BlockGetDPP()
  
  This function retrieves the device provision data from Device
  Provisioning Partition of GPP.
     
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @param[out] Data          A pointer to caller allocated buffer that will
                            store retrieved provision data. If NULL is
                            passed in, this function will return the 
                            provision data size.
  @param[out] DataSize      Upon successful return, the size of provision
                            data.
  
  @return
  EFI_SUCCESS               Provision data is retrieved from DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_OUT_OF_RESOURCES      No enough system resource.
  EFI_NOT_FOUND             Specified provision data not found.
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)
 
**/
EFI_STATUS
BlockGetDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN OUT VOID           *Data,
  IN OUT UINTN          *DataSize
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  CHAR16                 *pFilePath = NULL;
  UINTN                   nTypeLen = 0;
  UINTN                   nNameLen = 0;
    
  if((Type == NULL) ||
     (Name == NULL) ||
     (DataSize == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockGetDPPExit;
  }
    
  nTypeLen = StrLen(Type);
  nNameLen = StrLen(Name);

  if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
      (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockGetDPPExit;
  }

  // construct file name
  pFilePath = AllocateZeroPool(sizeof(CHAR16) * (nTypeLen + nNameLen + StrSize(L"\\")));
  if (pFilePath != NULL)
  {
    StrCat(pFilePath, Type);
    StrCat(pFilePath, L"\\");
    StrCat(pFilePath, Name);
  }
  else
  {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((EFI_D_ERROR, "[DppDxe] Could not allocate memory for file name. Status = 0x%x.\n", Status));
    goto BlockGetDPPExit;
  }

  // get file
  Status = gRawFSProtocol->FileRead(gRawFSProtocol, pFilePath, Data, DataSize, NULL, &gEfiDppBlockPartitionGuid, gDiskTypeGuid);

  // If Raw FS protocol could not read file, print an error message and return
  if(EFI_ERROR(Status))
  {
    if (Status == EFI_VOLUME_CORRUPTED)
    {
      DEBUG((EFI_D_ERROR, "[DppDxe] DPP partition header is corrupted! Partition needs to be re-formatted.\n"));
      goto BlockGetDPPExit;
    }
    else
    {
      DEBUG((EFI_D_ERROR, "[DppDxe] Failed to read DPP block data with RawFSProtocol.\n"));
    }
  }
  
BlockGetDPPExit:
    if (pFilePath != NULL)
    {
      FreePool(pFilePath);
      pFilePath = NULL;
    }

  return Status; 
}

/**

  BlockDeleteItemDPP()
  
  This function deletes the device provision data from Device
  Provisioning Partition of GPP.
   
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @return
  EFI_SUCCESS               Provision data is deleted from DPP 
                            successfully. 
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_NOT_FOUND             Specified provision data not found. 
  EFI_XXXX                  Other errors. (Refer to UEFI spec 
                            for details) 
**/
EFI_STATUS
BlockDeleteItemDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  CHAR16                 *pFilePath = NULL;
  UINTN                   nTypeLen = 0;
  UINTN                   nNameLen = 0;

  // Check input parameters
  if ((Type == NULL) || (Name == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockDeleteDPPExit;
  }
  
    nTypeLen = StrLen(Type);
    nNameLen = StrLen(Name);

    if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
        (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto BlockDeleteDPPExit;
  }

  // construct file name
  pFilePath = AllocateZeroPool(sizeof(CHAR16) * (nTypeLen + nNameLen + StrSize(L"\\")));
  if (pFilePath != NULL)
  {
    StrCat(pFilePath, Type);
    StrCat(pFilePath, L"\\");
    StrCat(pFilePath, Name);
  }
    else
  {
    Status = EFI_OUT_OF_RESOURCES;
        DEBUG((EFI_D_ERROR, "[DppDxe] Could not allocate memory for file name. Status = 0x%x.\n", Status));
    goto BlockDeleteDPPExit;
  }

  // delete file
  Status = gRawFSProtocol->FileDelete(gRawFSProtocol, pFilePath, NULL, &gEfiDppBlockPartitionGuid, gDiskTypeGuid);

    // If Raw FS protocol could not delete file, print an error message and return
  if(EFI_ERROR(Status))
  {
        if (Status == EFI_VOLUME_CORRUPTED)
  {
            DEBUG((EFI_D_ERROR, "[DppDxe] DPP partition header is corrupted! Partition needs to be re-formatted.\n"));
    goto BlockDeleteDPPExit;
  }
        else {
            DEBUG((EFI_D_ERROR, "[DppDxe] Failed to delete DPP block data with RawFSProtocol.\n"));
    }
  }

BlockDeleteDPPExit:
    if (pFilePath != NULL)
    {
        FreePool(pFilePath);
        pFilePath = NULL;
    }

  return Status;

}

/**

  FatSetDPP()

  Stores device provision data into block-based DPP.

   
  @param[in]  Type          An unicode string specifying provision data type,
                            e.g. "QCOM" or "OEM".
  @param[in]  Name          An unicode string specifying provision data name,
                            e.g. "WLAN.PROVISION"
  @param[in]  Data          A pointer to the buffer that contains provision data
                            to be written.
  @param[in]  DataSize      The size of provision data, in bytes.

  @return
  EFI_SUCCESS               Provision data is written to DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter.
  EFI_OUT_OF_RESOURCES      No enough system resource 
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)

**/
EFI_STATUS
FatSetDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN VOID               *Data,
  IN UINTN               DataSize
  )
{

  EFI_STATUS                        Status       = EFI_SUCCESS;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs           = NULL;
  EFI_FILE_PROTOCOL                *FsRoot       = NULL;
  EFI_FILE_PROTOCOL                *File         = NULL;
  EFI_FILE_INFO                    *FileInfo     = NULL;
  UINTN                             FileInfoSize = 0;
  volatile UINT64                   OpenMode     = 0;
  UINT64                            Attributes   = 0x0;
  EFI_GUID                          FileInfoGuid = EFI_FILE_INFO_ID ;
  CHAR16                           *pFilePath    = NULL; 
  UINTN                             nTypeLen     = 0;
  UINTN                             nNameLen     = 0;

  if ((Type == NULL) || 
      (Name == NULL) || 
      (Data == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto FatSetDPPExit;
  }
  
  nTypeLen = StrLen(Type);
  nNameLen = StrLen(Name);

  if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
      (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto FatSetDPPExit;
  }
  /* allocate buffer for CHAR16 file path */
  pFilePath = AllocateZeroPool(sizeof(CHAR16) * (nTypeLen + nNameLen +StrSize (L"\\\\")));
  if (pFilePath != NULL)
  {
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Type);
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Name);
  }
  else
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FatSetDPPExit;
  }

  Status = gBS->HandleProtocol(gDPPFatHandle,
                              &gEfiSimpleFileSystemProtocolGuid,
                              (VOID **)&Fs);
  HANDLE_ERROR_LABEL(Status, gBS->HandleProtocol, FatSetDPPExit);

  Status = Fs->OpenVolume(Fs,&FsRoot);
  HANDLE_ERROR_LABEL(Status, Fs->OpenVolume, FatSetDPPExit);

  if (FsRoot != NULL)
  {
    /* concat dir name and file name */
    OpenMode = EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE;

    Status = FsRoot->Open(FsRoot,
                          &File,
                          pFilePath,                          
                          OpenMode, 
                          Attributes);

    if(EFI_ERROR(Status))
    {
      if(Status == EFI_NOT_FOUND)
      {
        //Tried to write file to nonexistent directory,
        //need to create the directory and try again.
        Status = CreateFatDirectory(FsRoot,
                                    pFilePath,
                                    Attributes);
        HANDLE_ERROR_LABEL(Status, CreateFatDirectory, FatSetDPPExit);

        Status = FsRoot->Open(FsRoot,
                              &File,
                              pFilePath,                          
                              OpenMode,
                              Attributes);
        HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatSetDPPExit);
      }
      else
      {
        HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatSetDPPExit);
      }
    }
  }
  else
  {
    Status = EFI_NOT_FOUND;
    HANDLE_ERROR_LABEL(Status, Fs->OpenVolume, FatSetDPPExit);
  }

  if (File != NULL)
  {
    Status = GetFatFileInfo(File,&FileInfoSize,&FileInfo);
         
    if((!EFI_ERROR(Status)) && (FileInfo != NULL))
    {
      //Clear out the read only attribute
      if((FileInfo->Attribute) & EFI_FILE_READ_ONLY)
      {
        FileInfo->Attribute = (FileInfo->Attribute) & (0xFFFFFFFFFFFFFFFE) ;

        /* Validate and correct the FileInfo time is required before callig 
                   setInfo function. Also, igore the return status if 
                   FwUpdateValidateAndCorrectFileInfoTime returns error */
        ValidateAndCorrectFileInfoTime(&FileInfo->CreateTime);
        ValidateAndCorrectFileInfoTime(&FileInfo->ModificationTime);
        ValidateAndCorrectFileInfoTime(&FileInfo->LastAccessTime);

        Status = File->SetInfo (File, &FileInfoGuid, FileInfo->Size, FileInfo);
      }
    }

    Status = File->Write(File,
                         &DataSize,
                         Data);
    HANDLE_ERROR_LABEL(Status, File->Write, FatSetDPPExit);

    if(FileInfo != NULL)
    {
      gBS->FreePool(FileInfo);
      FileInfo = NULL;
    }
   
    Status = GetFatFileInfo(File,&FileInfoSize,&FileInfo);
    HANDLE_ERROR_LABEL(Status, GetFatFileInfo, FatSetDPPExit);

    if(FileInfo != NULL)
    {
      //Clear out the read only attribute
      if((FileInfo->Attribute) & EFI_FILE_READ_ONLY)
      {
        FileInfo->Attribute = (FileInfo->Attribute) & (0xFFFFFFFFFFFFFFFE) ;
      }
      FileInfo->FileSize = DataSize;
        
      /* Validate and correct the FileInfo time is required before callig 
               setInfo function. Also, igore the return status if 
              FwUpdateValidateAndCorrectFileInfoTime returns error */
      ValidateAndCorrectFileInfoTime(&FileInfo->CreateTime);
      ValidateAndCorrectFileInfoTime(&FileInfo->ModificationTime);
      ValidateAndCorrectFileInfoTime(&FileInfo->LastAccessTime);
        
      Status = File->SetInfo (File, &FileInfoGuid, FileInfo->Size, FileInfo);
      HANDLE_ERROR_LABEL(Status, File->SetInfo, FatSetDPPExit);

      Status = File->Flush(File);
      HANDLE_ERROR_LABEL(Status, File->Flush, FatSetDPPExit);
    }
  }
  else
  {
    Status = EFI_NOT_FOUND;
    HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatSetDPPExit);

  }

FatSetDPPExit:

  if(FileInfo)
  {
    gBS->FreePool(FileInfo);
    FileInfo = NULL;
  }

  if(File)
  {
    File->Close(File);
    File = NULL; 
  }
    
  if (FsRoot)
  {
    FsRoot->Close(FsRoot);
    FsRoot = NULL;
  }
  
  if (pFilePath)
  {
    FreePool(pFilePath);
    pFilePath = NULL;
  }

  return Status;
}

/**

  FatGetDPP()
  
  This function retrieves the device provision data from Device
  Provisioning Partition of GPP.
     
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @param[out] Data          A pointer to caller allocated buffer that will
                            store retrieved provision data. If NULL is
                            passed in, this function will return the 
                            provision data size.
  @param[out] DataSize      Upon successful return, the size of provision
                            data.
  
  @return
  EFI_SUCCESS               Provision data is retrieved from DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_OUT_OF_RESOURCES      No enough system resource.
  EFI_NOT_FOUND             Specified provision data not found.
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)
 
**/
EFI_STATUS
FatGetDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN OUT VOID           *Data,
  IN OUT UINTN          *DataSize
  )
{

  EFI_STATUS                        Status       = EFI_SUCCESS;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs           = NULL;
  EFI_FILE_PROTOCOL                *FsRoot       = NULL;
  EFI_FILE_PROTOCOL                *File         = NULL;
  EFI_FILE_INFO                    *FileInfo     = NULL;
  UINTN                             FileInfoSize = 0;
  UINT64                            Attributes   = 0x0;
  CHAR16                           *pFilePath    = NULL; 
  UINTN                             nTypeLen     = 0;
  UINTN                             nNameLen     = 0;


  if ((Name == NULL) || 
      (Type == NULL) || 
      (DataSize == NULL))
  { 
    Status = EFI_INVALID_PARAMETER;
    goto FatGetDPPExit;
   }

  nTypeLen = StrLen(Type);
  nNameLen = StrLen(Name);

  if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
      (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto FatGetDPPExit;
  }

  pFilePath = AllocateZeroPool ( 2*(nTypeLen + nNameLen + 2 * StrSize (L"\\")));
  if (pFilePath != NULL)
  {
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Type);
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Name);
  }
  else
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FatGetDPPExit;
  }
  
  Status = gBS->HandleProtocol(gDPPFatHandle,
                              &gEfiSimpleFileSystemProtocolGuid,
                              (VOID **)&Fs);
  HANDLE_ERROR_LABEL(Status, gBS->HandleProtocol, FatGetDPPExit);

  Status = Fs->OpenVolume(Fs,&FsRoot);
  HANDLE_ERROR_LABEL(Status, Fs->OpenVolume, FatGetDPPExit); 

  if (FsRoot != NULL)
  {
    Status = FsRoot->Open(FsRoot,
                          &File,
                          pFilePath,                          
                          EFI_FILE_MODE_READ,
                          Attributes);
    HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatGetDPPExit); 
  }
  else
  {
    Status = EFI_NOT_FOUND;
    HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatGetDPPExit); 
  }

  Status = GetFatFileInfo(File,&FileInfoSize,&FileInfo);
  HANDLE_ERROR_LABEL(Status, GetFatFileInfo, FatGetDPPExit);

  if ( (FileInfo != NULL) && (File != NULL))
  {  
    if (Data == NULL) /* return the required buffer size so that caller can 
                         allocate the buffer and call again*/
    {
        *DataSize = FileInfo->FileSize;
        Status = EFI_SUCCESS;
        goto FatGetDPPExit;
    }
    if (*DataSize < FileInfo->FileSize)
    {
      Status = EFI_BUFFER_TOO_SMALL; 
      goto FatGetDPPExit;
    }
    else
    {
      ZeroMem(Data, *DataSize);

      Status = File->Read(File,
                          DataSize,
                          Data);
      HANDLE_ERROR_LABEL(Status, File->Read, FatGetDPPExit);
    }
  }
  else
  {
    Status = EFI_NOT_FOUND;
    HANDLE_ERROR_LABEL(Status, FwUpdateFatGetFileInfo, FatGetDPPExit);
  }
FatGetDPPExit:

  if(FileInfo)
  {
    gBS->FreePool(FileInfo);
    FileInfo = NULL;
  }

  if (File)
  {
    File->Close(File);
    File = NULL; 
  }
    
  if (FsRoot)
  {
    FsRoot->Close(FsRoot);
    FsRoot = NULL;
  }

  if (pFilePath)
  {
    FreePool(pFilePath);
    pFilePath = NULL;
  }

  return Status;

}

/**

  FatDeleteItemDPP()
  
  This function deletes the device provision data from Device
  Provisioning Partition of GPP.
   
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @return
  EFI_SUCCESS               Provision data is deleted from DPP 
                            successfully. 
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_NOT_FOUND             Specified provision data not found. 
  EFI_XXXX                  Other errors. (Refer to UEFI spec 
                            for details) 
**/
EFI_STATUS
FatDeleteItemDPP(
  IN CHAR16             *Type,
  IN CHAR16             *Name
  )
{

  EFI_STATUS                         Status       = EFI_SUCCESS;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Fs           = NULL;
  EFI_FILE_PROTOCOL                 *FsRoot       = NULL;
  EFI_FILE_PROTOCOL                 *File         = NULL;
  EFI_FILE_INFO                     *FileInfo     = NULL;
  volatile UINT64                    OpenMode;
  UINT64                             Attributes   = 0x0;
  EFI_GUID                           FileInfoGuid = EFI_FILE_INFO_ID ;
  UINTN                              FileInfoSize = 0;
  UINTN                              FileSize     = 0;
  VOID                              *Buffer       = NULL;
  CHAR16                            *pFilePath    = NULL; 
  UINTN                              nTypeLen     = 0;
  UINTN                              nNameLen     = 0;


  if((Type == NULL) ||
     (Name == NULL))

  {
    Status = EFI_INVALID_PARAMETER;
    goto FatDeleteItemDPPExit;
  }
  nTypeLen = StrLen(Type);
  nNameLen = StrLen(Name);

  if ((nTypeLen > DPP_DATA_TYPE_MAX_LENGTH) ||
      (nNameLen > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto FatDeleteItemDPPExit;
  }

  pFilePath = AllocateZeroPool ( 2 * (nTypeLen + nNameLen +StrSize (L"\\\\")));
  if (pFilePath != NULL)
  {
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Type);
    StrCat (pFilePath, L"\\");
    StrCat (pFilePath, Name);
  }
  else
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FatDeleteItemDPPExit;
  }
  
  Status = gBS->HandleProtocol(gDPPFatHandle,
                              &gEfiSimpleFileSystemProtocolGuid,
                              (VOID **)&Fs);

  HANDLE_ERROR_LABEL(Status, gBS->HandleProtocol, FatDeleteItemDPPExit);

  Status = Fs->OpenVolume(Fs,&FsRoot);
  HANDLE_ERROR_LABEL(Status, Fs->OpenVolume, FatDeleteItemDPPExit);

  // Open the file as read and not write in case the file is marked with read-only attribute
  OpenMode = EFI_FILE_MODE_READ;

  Status = FsRoot->Open(FsRoot,
                        &File,
                        pFilePath,                          
                        OpenMode, 
                        Attributes);

  if (Status  == EFI_NOT_FOUND)
  {
    return EFI_SUCCESS;
  }

  HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatDeleteItemDPPExit);

  // First attempt to clear any read-only attribute which would prevent deleting the file   

  Status = GetFatFileInfo(File,&FileInfoSize,&FileInfo);
       
  if(!EFI_ERROR(Status) && (FileInfo != NULL))
  {
    if((FileInfo->Attribute) & EFI_FILE_READ_ONLY)
    {
      FileInfo->Attribute = (FileInfo->Attribute) & (0xFFFFFFFFFFFFFFFE) ;

      /* Validate and correct the FileInfo time is required before callig 
               setInfo function. Also, igore the return status if 
              FwUpdateValidateAndCorrectFileInfoTime returns error */
      ValidateAndCorrectFileInfoTime(&FileInfo->CreateTime);
      ValidateAndCorrectFileInfoTime(&FileInfo->ModificationTime);
      ValidateAndCorrectFileInfoTime(&FileInfo->LastAccessTime);

      File->SetInfo (File, &FileInfoGuid, FileInfo->Size, FileInfo);
      HANDLE_ERROR_LABEL(Status, File->SetInfo, FatDeleteItemDPPExit);
    }
    gBS->FreePool(FileInfo);
    FileInfo = NULL;
  }
  
  // Close and re-open the file with read & write to allow deleting
  File->Close(File);

  OpenMode = EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE;

  Status = FsRoot->Open(FsRoot,
                        &File,
                        pFilePath,                          
                        OpenMode,
                        Attributes);
  HANDLE_ERROR_LABEL(Status, FsRoot->Open, FatDeleteItemDPPExit);

  //read out the file size and zero out the file content before deletion
  Status = GetFatFileInfo(File,&FileInfoSize,&FileInfo);
       
  if(!EFI_ERROR(Status) && (FileInfo != NULL))
  {
    FileSize= FileInfo->FileSize;

    Status = gBS->AllocatePool(EfiBootServicesData, FileSize, (VOID**)&Buffer); 
    if (!EFI_ERROR(Status) && (Buffer != NULL))
    {
      ZeroMem(Buffer,FileSize);

      Status = File->Write(File,&FileSize, Buffer);
      HANDLE_ERROR_LABEL(Status, File->Write, FatDeleteItemDPPExit);

      Status = File->Flush(File);
      HANDLE_ERROR_LABEL(Status, File->Flush, FatDeleteItemDPPExit);

      Status = gBS->FreePool(Buffer);
      HANDLE_ERROR_LABEL(Status, gBS->FreePool, FatDeleteItemDPPExit);

      Buffer = NULL;
    }
    else
    {
      HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,FatDeleteItemDPPExit);
    }
    gBS->FreePool(FileInfo);
    FileInfo = NULL;
  }

  Status = File->Delete(File) ;
  if(!EFI_ERROR(Status))
  {
    File = NULL;
  }
  HANDLE_ERROR_LABEL(Status, File->Delete, FatDeleteItemDPPExit);

  FsRoot->Close(FsRoot);
  if(!EFI_ERROR(Status))
  {
    FsRoot = NULL;
  }

FatDeleteItemDPPExit:

  if(Buffer)
  {
    gBS->FreePool(Buffer);
    Buffer = NULL;
  }

  if(FileInfo)
  {
    gBS->FreePool(FileInfo);
    FileInfo = NULL;
  }

  if (File)
  {
    File->Close(File);
    File = NULL; 
  }
    
  if (FsRoot)
  {
    FsRoot->Close(FsRoot);
    FsRoot = NULL;
  }
  
  if (pFilePath)
  {
    FreePool(pFilePath);
    pFilePath = NULL;
  }
  return Status;

}

/**

  EraseDPP()

  Erase entire DPP partition.
   
  @param[in]  This          A pointer to the calling context. 
   
  @return
  EFI_SUCCESS               Successfully write DPP block data.
  EFI_OUT_OF_RESOURCES      No enough system resource 
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)

  Note:

  1. Given the DPP size, erasing entire DPP is a time-consuming operation.

**/

EFI_STATUS
EraseDPP(
  IN  EFI_DPP_PROTOCOL  *This
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
        Status = gRawFSProtocol->FormatFileSystem(gRawFSProtocol, DEFAULT_FS_BLOCKSIZE, 1, NULL, &gEfiDppBlockPartitionGuid, gDiskTypeGuid);

        // If Raw FS protocol could not erase DPP, try legacy block IO based implementation
        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "[DppDxe] Failed to erase DPP with RawFSProtocol. Status = 0x%x\r\n", Status));
    }
    }
  else
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("[DppDxe] Erase DPP unsupported on existing DPP",EraseDPPExit);
  }
EraseDPPExit:
  return Status;
}

/**

  SetDPP()

  Stores device provision data into DPP.

  @param[in]  This          A pointer to the calling context. 
   
  @param[in]  Type          An unicode string specifying provision data type,
                            e.g. "QCOM" or "OEM".
  @param[in]  Name          An unicode string specifying provision data name,
                            e.g. "WLAN.PROVISION"
  @param[in]  Data          A pointer to the buffer that contains provision data
                            to be written.
  @param[in]  DataSize      The size of provision data, in bytes.

  @return
  EFI_SUCCESS               Provision data is written to DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter.
  EFI_OUT_OF_RESOURCES      No enough system resource 
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)

**/
EFI_STATUS
SetDPP(
  IN  EFI_DPP_PROTOCOL  *This,
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN VOID               *Data,
  IN UINTN               DataSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if ((Type == NULL) ||
      (Name == NULL) ||
      (Data == NULL) ||
      (DataSize == 0))
  {
    Status = EFI_INVALID_PARAMETER;
    goto SetDPPExit;
  }
  
  if ((StrLen(Type) > DPP_DATA_TYPE_MAX_LENGTH) ||
      (StrLen(Name) > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto SetDPPExit;
  }

  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
    Status = BlockSetDPP(Type,Name,Data,DataSize);
    HANDLE_ERROR_LABEL(Status,BlockSetDPP,SetDPPExit);
  }
  else if (gDppPartType == DPP_PARTITION_TYPE_FAT)
  {
    Status = FatSetDPP(Type,Name,Data,DataSize);
    HANDLE_ERROR_LABEL(Status,FatSetDPP,SetDPPExit);
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("[DppDxe] Unsupported DPP type",SetDPPExit);
  }

SetDPPExit:

  return Status;
}

/**

  GetDPP()
  
  This function retrieves the device provision data from Device
  Provisioning Partition of GPP.
  
  @param[in]  This          A pointer to the calling context.
   
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @param[out] Data          A pointer to caller allocated buffer that will
                            store retrieved provision data. If NULL is
                            passed in, this function will return the 
                            provision data size.
  @param[out] DataSize      Upon successful return, the size of provision
                            data.
  
  @return
  EFI_SUCCESS               Provision data is retrieved from DPP successfully.
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_OUT_OF_RESOURCES      No enough system resource.
  EFI_NOT_FOUND             Specified provision data not found.
  EFI_XXXX                  Other errors. (Refer to UEFI spec for details)
 
**/
EFI_STATUS
GetDPP(
  IN  EFI_DPP_PROTOCOL  *This,
  IN CHAR16             *Type,
  IN CHAR16             *Name,
  IN OUT VOID           *Data,
  IN OUT UINTN          *DataSize
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
    
  if((Type == NULL) ||
     (Name == NULL) ||
     (DataSize == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto GetDPPExit;
  }
    
  if((StrLen(Type) > DPP_DATA_TYPE_MAX_LENGTH) ||
     (StrLen(Name) > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto GetDPPExit;
  }

  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
    Status = BlockGetDPP(Type,Name,Data,DataSize);
    HANDLE_ERROR_LABEL(Status,BlockGetDPP,GetDPPExit);
  }
  else if (gDppPartType == DPP_PARTITION_TYPE_FAT)
  {
    Status = FatGetDPP(Type,Name,Data,DataSize);
    HANDLE_ERROR_LABEL(Status,FatGetDPP,GetDPPExit);
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("[DppDxe] Unsupported DPP type",GetDPPExit);
  }

GetDPPExit:

  return Status;
}

/**

  DeleteItemDPP()
  
  This function deletes the device provision data from Device
  Provisioning Partition of GPP.
  
  @param[in]  This          A pointer to the calling context.
   
  @param[in]  Type          An unicode string of provision data type,
                            e.g. L"QCOM" or L"OEM".
  @param[in]  Name          An unicode string of provision data name,
                            e.g. L"WLAN.PROVISION".
  @return
  EFI_SUCCESS               Provision data is deleted from DPP 
                            successfully. 
  EFI_INVALID_PARAMETER     Invalid parameter. 
  EFI_NOT_FOUND             Specified provision data not found. 
  EFI_XXXX                  Other errors. (Refer to UEFI spec 
                            for details) 
 
**/
EFI_STATUS
DeleteItemDPP(
  IN  EFI_DPP_PROTOCOL  *This,
  IN CHAR16             *Type,
  IN CHAR16             *Name
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;

  // Check input parameters
  if ((Type == NULL) || (Name == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto DeleteDPPExit;
  }
  
  if ((StrLen(Type) > DPP_DATA_TYPE_MAX_LENGTH) ||
      (StrLen(Name) > DPP_DATA_NAME_MAX_LENGTH))
  {
    Status = EFI_INVALID_PARAMETER;
    goto DeleteDPPExit;
  }


  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
    Status = BlockDeleteItemDPP(Type,Name);
    HANDLE_ERROR_LABEL(Status,BlockSetDPP,DeleteDPPExit);
  }
  else if (gDppPartType == DPP_PARTITION_TYPE_FAT)
  {
    Status = FatDeleteItemDPP(Type,Name);
    HANDLE_ERROR_LABEL(Status,FatSetDPP,DeleteDPPExit);
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("[DppDxe] Unsupported DPP type",DeleteDPPExit);
  }
  
DeleteDPPExit:

  return Status;
}

/**
  UEFI DPP Protocol implementation
 */
static EFI_DPP_PROTOCOL gEfiDppProtocol = 
{
  EFI_DPP_PROTOCOL_REVISION,
  SetDPP,
  GetDPP,
  EraseDPP,
  DeleteItemDPP
};

/** 
  
  BlockDppProtocolInit()
  
  Initializes Block-based DPP protocol
 
  @return 
  EFI_SUCCESS               Raw File System protocol initialized successfully.
  EFI_UNSUPPORTED           Raw File System protocol not supported.
  EFI_XXXXXX                Failure occured. (Refer to UEFI spec for details) 
   
**/

EFI_STATUS
BlockDppProtocolInit()
{
    EFI_STATUS  Status = EFI_SUCCESS;
  
  // set gDiskTypeGuid
  if (boot_from_ufs())
    {
    gDiskTypeGuid = &gEfiUfsLU4Guid;
    }
  else if (boot_from_nvme())
    {
    gDiskTypeGuid = &gEfiSpiNor0Guid;
    }
    
  // Get Raw File System protocol interface to DPP handle
  Status = gBS->LocateProtocol(
    &gEfiRawFileSystemProtocolGuid,
    NULL,
    (VOID **)&gRawFSProtocol);
    
    return Status;
}

/** 
  
  FatDppProtocolInit()
  
  Initializes Fat-based DPP protocol
 
  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.  
  @param[in]  SystemTable   A pointer to the EFI System Table.
    
  @return 
  EFI_SUCCESS               DPP protocol initialized successfully.
  EFI_UNSUPPORTED           DPP protocol not supported.
  EFI_XXXXXX                Failure occured. (Refer to UEFI spec for details) 
   
**/

EFI_STATUS
FatDppProtocolInit(
  IN EFI_HANDLE DppHandle
  )
{
    EFI_STATUS  Status = EFI_SUCCESS;
  
    if(DppHandle == NULL)
    {
      Status = EFI_UNSUPPORTED;
      goto FatDppProtocolInit;
    }
    
    gDPPFatHandle = DppHandle;

FatDppProtocolInit:

    return Status;
}

/** 
  
  InitializeDppProtocol()
  
  Initializes DPP protocol
 
  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.  
  @param[in]  SystemTable   A pointer to the EFI System Table.
    
  @return 
  EFI_SUCCESS               DPP protocol initialized successfully.
  EFI_UNSUPPORTED           DPP protocol not supported.
  EFI_XXXXXX                Failure occured. (Refer to UEFI spec for details) 
   
**/

EFI_STATUS
InitializeDppProtocol(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_HANDLE  PartHandle = NULL;
    
    // If there is a block IO handle to a DPP partition, use block-based implementation
  Status = GetPartitionHandle(&PartHandle,
                              &gEfiDppBlockPartitionGuid,
                              NULL,
                              &gEfiBlockIoProtocolGuid);
  if(!EFI_ERROR(Status) && (PartHandle != NULL))
  {
    Status = BlockDppProtocolInit();
    HANDLE_ERROR_LABEL(Status, BlockDppProtocolInit, InitializeDppProtocolExit);
    gDppPartType = DPP_PARTITION_TYPE_BLOCK;

  }
    else if (Status == EFI_NOT_FOUND)
  {
    Status = GetPartitionHandle(&PartHandle,
                                &gEfiDppFatPartitionGuid,
                                NULL,
                                &gEfiSimpleFileSystemProtocolGuid);
    if(!EFI_ERROR(Status) && (PartHandle != NULL))
    {
      Status = FatDppProtocolInit(PartHandle);
      HANDLE_ERROR_LABEL(Status, FatDppProtocolInit, InitializeDppProtocolExit);
      gDppPartType = DPP_PARTITION_TYPE_FAT;
    }
    else
    {
      Status = EFI_UNSUPPORTED;
      DEBUG_ERR_LABEL("[DppDxe] Warning: DPP not found, please check if DPP is formated \n",
                      InitializeDppProtocolExit);
    }
  }
  else
  {
     Status = EFI_UNSUPPORTED;
     DEBUG_ERR_LABEL("[DppDxe] Warning: Block DPP unsupported platform type \n",InitializeDppProtocolExit);
  }

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
                                                  &gEfiDppProtocolGuid,
                                                  (void *)&gEfiDppProtocol,
                                                  NULL);
  HANDLE_ERROR_LABEL(Status, gBS->InstallMultipleProtocolInterfaces, InitializeDppProtocolExit);

  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
    DEBUG((EFI_D_INFO, "[DppDxe] Block"));
  }
  else if (gDppPartType == DPP_PARTITION_TYPE_FAT)
  {
    DEBUG((EFI_D_INFO, "[DppDxe] FAT"));
  }
  else
  { /* shouldn't happen */
    DEBUG((EFI_D_INFO, "[DppDxe] Unknown"));
  }
  DEBUG((EFI_D_INFO, "[DppDxe] DPP protocol installed!\n"));
  
InitializeDppProtocolExit:
  return Status;
}
