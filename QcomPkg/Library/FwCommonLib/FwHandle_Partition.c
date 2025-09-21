/** @file PartitionHandles.c
   
  Handles core partition functions for FwCommonLib. 
   
  Maintains a registry of the partitions needed for FwUpdate 
    and provides lookup functions for these partitions and
    their appropriate read/write functions.

  Copyright (c) 2011-2012,2016-2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2017/04/18     gparam      Removed unused/redundant definitions/parameters
 2017/02/13     gpavithr    Removed the log message which prints ACTIVE LUN in GetBootLUN
 2016/12/07     ssumit      Replaced #define GUIDs with extern variables
 2016/06/27     edg         Modified FwUpdateGetEmmcPartitionTypeGuid function to be generic for UFS support
 2012/11/15     mic         Added FwUpdateGetEmmcPartitionTypeGuid function
 2012/10/17     rs          Changes in FwUpdateGetPartitionHandle to check for
                            Partition Name
 2012/07/10     rs          Changes in FindinFV to check all FWFile Enums
 2011/10/12     jthompso    Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/FwCommonLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/BootConfig.h>
#include <Library/DevicePathLib.h>
#include <Pi/PiDxeCis.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Include/Library/QcomLib.h>
#include <Include/api/storage/ufs_api.h>
#include <Include/Protocol/EFICardInfo.h>
#include "FwHandle_Partition.h"

/* UFS Device Path */
typedef struct {
   VENDOR_DEVICE_PATH  Ufs;
   EFI_DEVICE_PATH     End;
} QCUFS_DEVICE_PATH;

extern GUID gEfiPartitionTypeGuid;
extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gEfiPartitionRecordGuid;
extern EFI_GUID gEfiMemCardInfoProtocolGuid;
extern EFI_GUID gEfiEmmcUserPartitionGuid;
extern EFI_GUID gEfiEmmcBootPartition1Guid;
extern EFI_GUID gEfiEmmcBootPartition2Guid;
extern EFI_GUID gEfiEmmcRpmbPartitionGuid;
extern EFI_GUID gEfiEmmcGppPartition1Guid;
extern EFI_GUID gEfiEmmcGppPartition2Guid;
extern EFI_GUID gEfiEmmcGppPartition3Guid;
extern EFI_GUID gEfiEmmcGppPartition4Guid;
extern EFI_GUID gEfiUfsLU0Guid;
extern EFI_GUID gEfiUfsLU1Guid;
extern EFI_GUID gEfiUfsLU2Guid;
extern EFI_GUID gEfiUfsLU3Guid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gEfiUfsLU5Guid;
extern EFI_GUID gEfiUfsLU6Guid;
extern EFI_GUID gEfiUfsLU7Guid;

extern const QCOM_FW_UPDATE_FILE_IDENTIFIER FilePaths[];

CONST EFI_GUID *FwEntryDiskPartitionGuids[FWENTRY_DISK_PARTITION_MAX] = {
    &gEfiEmmcUserPartitionGuid,
    &gEfiEmmcBootPartition1Guid,
    &gEfiEmmcBootPartition2Guid,
    &gEfiEmmcRpmbPartitionGuid,
    &gEfiEmmcGppPartition1Guid,
    &gEfiEmmcGppPartition2Guid,
    &gEfiEmmcGppPartition3Guid,
    &gEfiEmmcGppPartition4Guid,
    &gEfiUfsLU0Guid,
    &gEfiUfsLU1Guid,
    &gEfiUfsLU2Guid,
    &gEfiUfsLU3Guid,
    &gEfiUfsLU4Guid,
    &gEfiUfsLU5Guid,
    &gEfiUfsLU6Guid,
    &gEfiUfsLU7Guid
};

// HandleRegistry keeps mapping of Partition Enums to EFI handles
// for the actual partitions.  
// Note: Each enum must match its corresponding entry's index in HandleRegistry.
static QCOM_FW_UPDATE_PARTITION_HANDLE HandleRegistry[QCOM_FW_UPDATE_PARTITION_MAX] = {
    { QCOM_FW_UPDATE_PARTITION_GPP_SSD, NULL, QCOM_FW_UPDATE_PARTITION_TYPE_BLOCK, QCOM_FW_UPDATE_PARTITION_TYPE_GPP_SSD, &gEfiUfsLU4Guid },
    { QCOM_FW_UPDATE_PARTITION_GPP_FAT16, NULL, QCOM_FW_UPDATE_PARTITION_TYPE_FAT, QCOM_FW_UPDATE_PARTITION_TYPE_GPP_FAT16, &gEfiUfsLU4Guid },
    { QCOM_FW_UPDATE_PARTITION_USER_ESP, NULL, QCOM_FW_UPDATE_PARTITION_TYPE_FAT, QCOM_FW_UPDATE_PARTITION_TYPE_USER_ESP, &gEfiUfsLU0Guid }
};

//These are the functions used to read a file from a partition
// Each partiton type needs its own separate method
// These are dispatched in CopyFwFile
VOID* ReadFileFunctions[QCOM_FW_UPDATE_PARTITION_TYPE_MAX] ={
    FwUpdateReadFileFromBlock,
    FwUpdateReadFileFromFat,
    FwUpdateReadFileFromFV,
    DummyFunction //Need to fill this in.
};

//These are the functions used to write a file to a partition
// Each partiton type needs its own separate method
// These are dispatched in CopyFwFile
VOID* WriteFileFunctions[QCOM_FW_UPDATE_PARTITION_TYPE_MAX] ={
    FwUpdateWriteFileToBlock,
    FwUpdateWriteFileToFat,
    DummyFunction, //Should never happen, will add bounds-check.
    DummyFunction // Need to fill this in
};


//These are the functions used to read a file from a partition
// Each partiton type needs its own separate method
// These are dispatched in CopyFwFile2
VOID* ReadFileFunctions2[QCOM_FW_UPDATE_PARTITION_TYPE_MAX] ={
    DummyFunction,
    FwUpdateReadFileFromFat2,
    DummyFunction,
    DummyFunction //Need to fill this in.
};

//These are the functions used to write a file to a partition
// Each partiton type needs its own separate method
// These are dispatched in CopyFwFile2
VOID* WriteFileFunctions2[QCOM_FW_UPDATE_PARTITION_TYPE_MAX] ={
    DummyFunction,
    FwUpdateWriteFileToFat2,
    DummyFunction, //Should never happen, will add bounds-check.
    DummyFunction // Need to fill this in
};

/**
 * Function to get a disk partition type guid by a given
 * disk partition type enum
 *  
 * @param DiskType      - Partition type enum
 * @Param DiskTypeGuid  - Disk type GUID
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetDiskPartitionTypeGuid(
    IN  FWENTRY_DISK_PARTITION_TYPE  DiskType,
    OUT EFI_GUID                    *DiskTypeGuid
    )
{
    if ((DiskType >=  FWENTRY_DISK_PARTITION_MAX) ||
        (DiskTypeGuid == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    CopyMem((VOID*)DiskTypeGuid, (VOID*)FwEntryDiskPartitionGuids[DiskType], sizeof(EFI_GUID));
    
    return EFI_SUCCESS;
  
}
/**
 * Function to get a matching partition handle
 * after comparing the Disk type, Partition Name and 
 * Partition type GUID
 *  
 * @param PartHandle    - partition handle
 * @param DiskType      - Disk partition type
 * @Param PartTypeGuid  - Partition type GUID
 * @Param PartitionName - Partition name
 * @Param Protocol      - Protocol
 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetPartitionHandle(
    IN OUT EFI_HANDLE              *PartHandle,
    IN FWENTRY_DISK_PARTITION_TYPE  DiskType,
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
    VENDOR_DEVICE_PATH        *VendorData         = NULL;
  
  
    if ((PartHandle == NULL) ||
        (DiskType >= FWENTRY_DISK_PARTITION_MAX) ||
        (PartTypeGuid == NULL) ||
  
        (PartitionName == NULL) ||
        (Protocol == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }
       
    Status = gBS->LocateHandleBuffer(ByProtocol,
                                    Protocol,
                                     NULL,
                                     &NumHandles,
                                     &HandleBuffer);
    HANDLE_ERROR_LABEL(Status,gBS->LocateHandleBuffer,FwUpdateGetPartitionHandleExit);
           
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
        VendorData = (VENDOR_DEVICE_PATH *)DevPathInst;
        DevPathInst = (EFI_DEVICE_PATH_PROTOCOL*)(((UINT8*)DevPathInst)+DevPathInst->Length[0]);
        
        if(DevPathInst->Type == TYPE_MEDIA_DEVICE_PATH)
        {    
            if ((CompareGuid(&(VendorData->Guid), (FwEntryDiskPartitionGuids[DiskType])))
             && (CompareGuid(TempPartTypeGuid,PartTypeGuid)) 
             && (CompareMem(TempPartitionName, PartitionName, StrSize(TempPartitionName)) == 0)) 
            {
                *PartHandle = HandleBuffer[HandleCounter];
                Status = EFI_SUCCESS;
                goto FwUpdateGetPartitionHandleExit;
            }
        }                   
    }
    
    Status = EFI_NOT_FOUND;
  
FwUpdateGetPartitionHandleExit:
    if(HandleBuffer != NULL)
    {
        gBS->FreePool(HandleBuffer);
        HandleBuffer = NULL;
    }

    return Status;
};

/**
 * Locates instances of a given
 *  protocol and adds them to the
 *  handle registry, if applicable.
 *  
 * @param Protocol - Protocol to locate. 
 * @return EFI_STATUS 
 */
EFI_STATUS
AddHandlesForProtocol(
    IN EFI_GUID *Protocol
    )
{

    EFI_STATUS                              Status;
    EFI_HANDLE                              *HandleBuffer;
    UINTN                                   NumHandles = 0;
    UINTN                                   HandleCounter;
    UINTN                                   RegistryCounter;
    EFI_GUID                                *PartType;   

    Status = gBS->LocateHandleBuffer(ByProtocol,
                            Protocol,
                            NULL,
                            &NumHandles,
                            &HandleBuffer);
    ERR_HANDLE(Status);

    for(RegistryCounter = 0; RegistryCounter < QCOM_FW_UPDATE_PARTITION_MAX; RegistryCounter++)
    {        
        if(HandleRegistry[RegistryCounter].Handle == NULL)
        {
            for(HandleCounter=0;HandleCounter<NumHandles;HandleCounter++)
            {   
                //Get Partition Type
                Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                                             &gEfiPartitionTypeGuid,
                                             (VOID**)&PartType);  

                if(EFI_ERROR(Status))
                {
                    continue;
                }

                if(CompareGuid(PartType,&(HandleRegistry[RegistryCounter].PartType)))
                {
                    HandleRegistry[RegistryCounter].Handle = HandleBuffer[HandleCounter];
                    break;
                }                    
            }
        }
    }
    return EFI_SUCCESS;
}   

/**
 * Function iterates through all
 *  handles to EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
 *  and EFI_BLOCK_IO_PROTOCOL and fills
 *  in the HandleRegistry.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
UpdateHandleRegistry(
    )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    //Add BlockIo Handles
    Status = AddHandlesForProtocol(&gEfiBlockIoProtocolGuid);
    ERR_HANDLE(Status);

    //Add FileSystem Handles
    Status = AddHandlesForProtocol(&gEfiSimpleFileSystemProtocolGuid);
    ERR_HANDLE(Status);

    return Status;
}


/**
 *  
 * Returns the Handle for the 
 * given partition from the HandleRegistry. 
 * 
 * 
 * @param Part - Partition Enum, the partition whose 
 *          handle; we want. 
 * @param Handle - Pointer to the output Handle.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateGetHandleForPartition(
  IN  QCOM_FW_UPDATE_FW_PARTITION Part,
  OUT EFI_HANDLE                  *Handle
  )
{
    if((Part > QCOM_FW_UPDATE_PARTITION_MAX) ||
       (Handle == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }
    
    if( HandleRegistry[Part].Handle != NULL)
    {
        *Handle = HandleRegistry[Part].Handle;   
        return EFI_SUCCESS;
    
    }
    else
    {
        *Handle = NULL;
        return EFI_INVALID_PARAMETER;
        /*To Do 
          Get partition type*/
        
        /*locate handle*/   
    }
}

/**
 * Looks up the Read function 
 *  for the appropriate partition 
 * 
 * 
 * @param Part - Partition enum
 * @param Fn - Pointer to Read() function.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateGetReadFunction(
    IN  QCOM_FW_UPDATE_FW_PARTITION Part,
    VOID                            **Fn
    )
{

    if(Part <QCOM_FW_UPDATE_PARTITION_MAX &&
       HandleRegistry[Part].Type <QCOM_FW_UPDATE_PARTITION_TYPE_MAX)
    {
        *Fn = ReadFileFunctions[HandleRegistry[Part].Type];   
        return EFI_SUCCESS;
    }
    
    return EFI_INVALID_PARAMETER;
}
/**
 * Looks up the Write function 
 *  for the appropriate partition. 
 * 
 * 
 * @param Part - Partition enum
 * @param Fn - Pointer to Write() function.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateGetWriteFunction(
    IN  QCOM_FW_UPDATE_FW_PARTITION Part,
    VOID                            **Fn
    )
{

    if(Part <QCOM_FW_UPDATE_PARTITION_MAX &&
       HandleRegistry[Part].Type <QCOM_FW_UPDATE_PARTITION_TYPE_MAX)
    {
        *Fn = WriteFileFunctions[HandleRegistry[Part].Type];   
        return EFI_SUCCESS;
    }
    
    return EFI_INVALID_PARAMETER;
}


/**
 * Looks up the Read function 
 *  for the appropriate partition 
 * 
 * 
 * @param Part - Partition enum
 * @param Fn - Pointer to Read() function.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateGetReadFunction2(
    IN  QCOM_FW_UPDATE_FW_PARTITION Part,
    VOID                            **Fn
    )
{

    if(Part <QCOM_FW_UPDATE_PARTITION_MAX &&
       HandleRegistry[Part].Type <QCOM_FW_UPDATE_PARTITION_TYPE_MAX)
    {
        *Fn = ReadFileFunctions2[HandleRegistry[Part].Type];   
        return EFI_SUCCESS;
    }
    
    return EFI_INVALID_PARAMETER;
}
/**
 * Looks up the Write function 
 *  for the appropriate partition. 
 * 
 * 
 * @param Part - Partition enum
 * @param Fn - Pointer to Write() function.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateGetWriteFunction2(
    IN  QCOM_FW_UPDATE_FW_PARTITION Part,
    VOID                            **Fn
    )
{

    if(Part <QCOM_FW_UPDATE_PARTITION_MAX &&
        HandleRegistry[Part].Type <QCOM_FW_UPDATE_PARTITION_TYPE_MAX)
    {
        *Fn = WriteFileFunctions2[HandleRegistry[Part].Type];   
        return EFI_SUCCESS;
    }

    return EFI_INVALID_PARAMETER;
}

/**
 * This function reads partition info into a buffer.
 * 
 * 
 * @param SrcPart - Enum for the source partition
 * @param PartInfo - Buffer to store partition info
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
ReadPartitionInfo(
    IN  QCOM_FW_UPDATE_FW_PARTITION SrcPart,
    OUT PPARTITION_INFO             PartInfo
    )
{
    EFI_STATUS Status;
    EFI_HANDLE PartHandle;

    Status = FwUpdateGetHandleForPartition(SrcPart,
                                           &PartHandle);

    ERR_HANDLE(Status);

    Status = FwUpdateGetPartitionInfo(PartHandle, PartInfo);

    ERR_HANDLE(Status);

    return Status;
}


/**
 * Reads block data from a partition.
 *
 * @param SrcPart - Enum for the source partition
 * @param Offset  - Logical block address to read from.
 * @param Length  - Number of bytes to read. Must be
 *                  aligned to block size.
 * @param Buffer  - Buffer to store the data. 
 *
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS
ReadPartitionBlocks(
    IN QCOM_FW_UPDATE_FW_PARTITION  SrcPart,
    IN UINT32                       Offset,
    IN OUT UINT32                   *Length,
    OUT VOID                        **Buffer
    )
{
    EFI_STATUS Status;
    EFI_HANDLE PartHandle;
    
    Status = FwUpdateGetHandleForPartition(SrcPart,
                                           &PartHandle);

    ERR_HANDLE(Status);

    Status = FwUpdateReadBlocks(PartHandle, Offset, Length, Buffer);

    ERR_HANDLE(Status);

    return Status;
}


/**
 * Writes block data to a partition.
 *
 * @param SrcPart - Enum for the source partition
 * @param Offset  - Logical block address to write to.
 * @param Length  - Number of bytes to write. Must be
 *                  aligned to block size.
 * @param Buffer  - Buffer to store the data.
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS
WritePartitionBlocks(
    IN QCOM_FW_UPDATE_FW_PARTITION  SrcPart,
    IN UINT32                       Offset,
    IN OUT UINT32                   *Length,
    IN VOID                         **Buffer
    )
{
    EFI_STATUS Status;
    EFI_HANDLE PartHandle;
    
    Status = FwUpdateGetHandleForPartition(SrcPart,
                                           &PartHandle);

    ERR_HANDLE(Status);

    Status = FwUpdateWriteBlocks(PartHandle, Offset, Length, Buffer);

    ERR_HANDLE(Status);

    return Status;
}

/**
 * Given a FAT directory path, retrieve the names of all files  
 * that immediately reside under specified path. This is not a 
 * recursive operation so subfolders are not pinged.
 * 
 * @param Part - Enumeration of FAT partition handle. 
 * @param FwFile - Enumeration of directory path. 
 * @param Path - Upon return, store the unicode string of 
 *             directory path
 * @param Subfolder - Upon return, store the bottom-level 
 *                subfolder name of specified directory path,
 *                e.g. if path is "\DPP\QCOM\", then return
 *                "QCOM".
 * @param FileList - Upon return, a pointer to a list of unicode 
 *                 Null-terminated file names.
 * @param FileCount - Upon return, the number of files. 
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
ReadFatFileList(
    IN QCOM_FW_UPDATE_FW_PARTITION  Part,
    IN QCOM_FW_UPDATE_FW_FILE       FwFile,
    OUT CHAR16                      **Path,
    OUT CHAR16                      **Subfolder,
    OUT CHAR16                      ***FileList,
    OUT UINTN                       *FileCount
    )
{
    EFI_STATUS Status;
    EFI_HANDLE PartHandle;

    Status = FwUpdateGetHandleForPartition(Part, &PartHandle);
  
    ERR_HANDLE(Status);

    Status = FwUpdateGetFileName(FwFile, Path);

    ERR_HANDLE(Status);

    Status = FwUpdateGetFileList(PartHandle,
                                 *Path,
                                 Subfolder,
                                 FileList,
                                 FileCount);

    ERR_HANDLE(Status);
    
    return Status;
}

/**
 * Given a FW File Guid, returns whether this belongs to 
 * the set of valid Bootloader or ACPI file GUIDs possible 
 * in a payload
 * 
 * @param FileGuid  - Enumeration of FWFile GUIDs. 
 * @param ValidGuid - return whether the FileGuid is a valid 
 *            bootloader GUID or not
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
isValidBootLoaderGuid(
  IN const EFI_GUID *FileGuid,
  OUT BOOLEAN *ValidGuid
  )
{
    EFI_STATUS Status      = EFI_SUCCESS;
    EFI_GUID NullGuid      = QCOM_FW_UPDATE_NULL_GUID;
    EFI_GUID MetaDataGuid  = QCOM_FW_UPDATE_FILE_VERSION_METADATA_IN_PAYLOAD_GUID;
    EFI_GUID UpdateAppGuid = QCOM_FW_UPDATE_FILE_UPDATE_APP_IN_PAYLOAD_GUID;

    if(CompareGuid(FileGuid,&NullGuid) || CompareGuid(FileGuid,&MetaDataGuid) 
       || CompareGuid(FileGuid,&UpdateAppGuid))
    {
        *ValidGuid = FALSE;
    }
    else
    {
        *ValidGuid = TRUE;
    }
    return Status;
}

/**
* Depending on the device flash type, EMMC or UFS, returns
* the GUID for the BOOT/LUN1/LUN2 and GPP/LUN4 partition
*
* @param BOOTGUID - GUID number for the BOOT/LUN1/LUN2 partition.
* @param GPPGUID  - GUID number for the GPP/LUN4 partition.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetPartitionGUIDs(
   OUT EFI_GUID  *BOOTGUID,
   OUT EFI_GUID  *GPPGUID
   )
{
    EFI_STATUS   Status        = EFI_SUCCESS;
    UINT32       BootLunNumber = 0;
    UINT32       LunNumber1    = 1;
    UINT32       LunNumber2    = 2;
    EFI_GUID     LUN1GUID      = gEfiUfsLU1Guid;
    EFI_GUID     LUN2GUID      = gEfiUfsLU2Guid;
    EFI_GUID     LUN4GUID      = gEfiUfsLU4Guid;
    EFI_GUID     emmcBootGUID  = gEfiEmmcBootPartition1Guid;
    EFI_GUID     emmcGPPGUID   = gEfiEmmcGppPartition1Guid;

    if (BOOTGUID == NULL || GPPGUID == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    /* Getting BOOTGUID and GPPGUID depending on device's flash type: eMMC or UFS */
    /* UFS device requires extra check to determine bootable LUN: LUN1 or LUN2    */
    if (boot_from_ufs())
    {
        Status = GetBootLUN(&BootLunNumber);
        HANDLE_ERROR_LABEL(Status, GetBootLUN, GetPartitionGUIDsExit);

        if (BootLunNumber == LunNumber1)
        {
            CopyMem((VOID*)BOOTGUID, (VOID*)&LUN1GUID, sizeof (EFI_GUID));
            CopyMem((VOID*)GPPGUID,  (VOID*)&LUN4GUID, sizeof (EFI_GUID));
            Status = EFI_SUCCESS;
        }
        else if (BootLunNumber == LunNumber2)
        {
            CopyMem((VOID*)BOOTGUID, (VOID*)&LUN2GUID, sizeof (EFI_GUID));
            CopyMem((VOID*)GPPGUID,  (VOID*)&LUN4GUID, sizeof (EFI_GUID));
            Status = EFI_SUCCESS;
        }
        else
        {
            Status = EFI_NOT_FOUND;
            DEBUG_ERR_LABEL("Could not determine Boot LUN", GetPartitionGUIDsExit);
        }
    }
    else if (boot_from_emmc())
    {
        CopyMem((VOID*)BOOTGUID, (VOID*)&emmcBootGUID, sizeof (EFI_GUID));
        CopyMem((VOID*)GPPGUID,  (VOID*)&emmcGPPGUID, sizeof (EFI_GUID));
        Status = EFI_SUCCESS;
    }
    else
    {
        Status = EFI_INCOMPATIBLE_VERSION;
        DEBUG_ERR_LABEL("Could not determine Boot flash type", GetPartitionGUIDsExit);
    }

GetPartitionGUIDsExit:

    return Status;
}

/**
* It searches for the MemCardInfo handler using gEfiMemCardInfoProtocolGuid protocol
*
* @param MemCardInfo  - Handler obtained by gEfiMemCardInfoProtocolGuid protocol.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetMemCardInfoHandle(
    OUT EFI_MEM_CARDINFO_PROTOCOL **MemCardInfo
   )
{
    EFI_STATUS            Status           = EFI_SUCCESS;
    UINTN                 NumHandles       = 0;
    EFI_HANDLE            *HandleBuffer    = NULL;
    UINTN                 HandleCounter;
    QCUFS_DEVICE_PATH     *DevicePath       = NULL;


    if (MemCardInfo == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateHandleBuffer(ByProtocol,
                                     &gEfiMemCardInfoProtocolGuid,
                                     NULL,
                                     &NumHandles,
                                     &HandleBuffer);
    ERR_HANDLE(Status);
    
    for(HandleCounter = 0; HandleCounter < NumHandles; HandleCounter++)
    {
        // Get MemCardInfo
        Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                                     &gEfiMemCardInfoProtocolGuid,
                                    (VOID**)MemCardInfo);  

        if(EFI_ERROR(Status))
        {
           continue;
        }

        // Compare MemCardInfo to UFS LUN1
        DevicePath = (QCUFS_DEVICE_PATH *) DevicePathFromHandle(HandleBuffer[HandleCounter]);
        
        if(DevicePath == NULL)
        {
            continue;
        }

        if (CompareGuid(&DevicePath->Ufs.Guid, (FwEntryDiskPartitionGuids[FWENTRY_UFS_LUN1]))) 
        {
            Status = EFI_SUCCESS;
            goto GetMemCardInfoHandleExit;
        }
    }

    Status = EFI_NOT_FOUND;

GetMemCardInfoHandleExit:
    if(HandleBuffer != NULL)
    {
        gBS->FreePool(HandleBuffer);
        HandleBuffer = NULL;
    }

    return Status;
}

/**
* Identifies the boot LUN number in a UFS device
*
* @param LUN  - UFS LUN number.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetBootLUN(
    OUT UINT32  *LUN
    )
{
    EFI_STATUS                Status           = EFI_SUCCESS;
    EFI_MEM_CARDINFO_PROTOCOL *MemCardInfo     = NULL;
    UINT32                    BootLUN          = 0;       

    if (LUN == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetMemCardInfoHandle(&MemCardInfo);
    HANDLE_ERROR_LABEL(Status, GetMemCardInfoHandle, GetBootLUNExit);

    Status = MemCardInfo->GetBootLU(MemCardInfo, &BootLUN);
    HANDLE_ERROR_LABEL(Status, MemCardInfo->GetBootLU, GetBootLUNExit);

    *LUN = BootLUN;

GetBootLUNExit:

    return Status;

}
