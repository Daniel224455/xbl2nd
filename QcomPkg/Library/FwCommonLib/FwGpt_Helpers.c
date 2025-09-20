/** @file FwGptHelpers.c
   
  GUID Partition Table (GPT) functions for FwCommonLib.

  Copyright (c) 2011-2016, 2018, Qualcomm Technologies, Inc . All rights reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/07/20     gparam      KW Fixes
 2018/07/11     prema       Changes to move GPT backup from file to NV
 2018/07/11     rajb        Added support to get active GPT data from main GPT & vice
                            versa. Implemented compression & decompression support 
                            for GPT data to store to NV
 2018/05/31     prema       Change to support capsule update for SPINOR-NVME devices
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2016/12/07     ssumit      Replaced #define GUIDs with extern variables
 2016/08/29     ltg         Added UFS support
 2015/04/15     mic         Added 64bit firmware update support
 2014/17/10     tj          KW Fixes
 2014/04/21     mic         Fixed KW warnings
 2012/12/11     mic         Fixed KW warnings
 2012/11/14     mic         Added robustness for GPT copy and restore functions
 2012/11/12     rs          Swap ParitionName also when switching GPT
 2012/10/10     jd          Handle GPT switch based on firmware entry list   
 2012/10/08     mic         renamed gpt functions
 2011/10/12     hozefak     Initial version

=============================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FwCommonLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include <Library/zlib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi/UefiGpt.h>
#include "FwGpt_Helpers.h"
#include "FwHandle_Partition.h"

#define TYPE_END_DEVICE_PATH       0x7F
#define PRIMARY_HEADER_LBA         0x1

/**
 * Finds the blockIOHandle for the given Vendor GUID which is 
 * defined in MMCHS driver 
 * 
 * @param Memory_GUID - Vendor GUID to find the BlockIOHandle 
 * @param hBlockIO    - BlockIO handle
 * @return EFI_STATUS
 */

EFI_STATUS
GetDiskTypeHandle(
    IN  EFI_GUID                Memory_GUID,
    OUT EFI_BLOCK_IO_PROTOCOL **hBlockIO
   )
{
    EFI_STATUS          Status        = EFI_SUCCESS;
    UINT32              Attribs       = 0;
    PartiSelectFilter   HandleFilter;
    // Use size as 2 to detect existance of more than one capsule partition while query
    HandleInfo          HandleInfoList [10];
    UINT32              NoHandles     = sizeof(HandleInfoList) / sizeof(*HandleInfoList);

    if (hBlockIO == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    /* Initialize output parameter*/
    *hBlockIO = NULL;

    Attribs |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
    Attribs |= BLK_IO_SEL_SELECT_ROOT_DEVICE_ONLY;
  
    HandleFilter.VolumeName = NULL;
    HandleFilter.RootDeviceType = &Memory_GUID;
    HandleFilter.PartitionType = NULL;
    HandleFilter.PartitionLabel = NULL;
    
    Status = GetBlkIOHandles(Attribs, &HandleFilter, HandleInfoList, &NoHandles);
    
    if ((EFI_ERROR (Status)) ||
        (0 == NoHandles)) {
      DEBUG ((EFI_D_ERROR, "Failed to find handle for given root device type. Status: %r\n", Status));
      Status = (EFI_ERROR (Status) ? Status : EFI_NOT_FOUND);
      goto GetDiskTypeHandleExit;
    }

#if 0  // TODO: Prem: Handle as per response from UEFI team
    if (NoHandles != 1) {
      DEBUG ((EFI_D_ERROR, "Error: More than 1 handle found for the given root device type.\n"));
      Status = EFI_UNSUPPORTED;
      goto GetDiskTypeHandleExit;
    }
#endif // #if 0

    *hBlockIO = HandleInfoList[0].BlkIo;

GetDiskTypeHandleExit:

    return Status;
}

/**
 * This function will write the Buffer into the specified GPT
 * Table. It will also calculate the CRC's and rewrite the
 * header for the modified table
 * 
 * @param FileBuffer  :  Pointer to Buffer which needs to be
 *                       written to the GPT Tables 
 * @param FileSize    :  Pointer to the Size of the buffer
 * @param Memory_GUID :  Vendor GUID whose GPT has to be changed
 * 
 * @param TableType   : Primary or backup GPT table
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS 
WriteGPTTable(
    IN VOID         **FileBuffer,
    IN UINTN         *FileSize,
    IN EFI_GUID       Memory_GUID,
    IN GPT_TABLE_TYPE TableType
    )
{
    EFI_STATUS                  Status                 = EFI_SUCCESS;
    UINT32                      CurrentLBA             = 0;
    EFI_PARTITION_ENTRY        *StartPartitionEntry    = NULL;
    EFI_PARTITION_ENTRY        *PartitionEntry         = NULL;
    EFI_PARTITION_TABLE_HEADER *PartHdr                = NULL;
    EFI_PARTITION_TABLE_HEADER *BackupPartHdr          = NULL;
    EFI_PARTITION_TABLE_HEADER *OutPartitionHeader     = NULL;
    UINT32                      Partition_Array_CRC32  = 0;
    UINT32                      CRCforHeader           = 0;
    UINT32                      BlocksToWrite          = 0;
    UINT32                      BlocksToWriteRemainder = 0;
    UINT8                      *BlocksToWriteBuffer    = NULL;
    UINT8                      *BlocksToWriteTempBuffer= NULL;
    EFI_BLOCK_IO_PROTOCOL      *BlockIoHandleHardDrive = NULL;
    UINT32                      OutStartLBA            = 0;
   
    /* Get the BlockIO handle for the given Disk type*/
    Status = GetDiskTypeHandle(Memory_GUID, &BlockIoHandleHardDrive);
    HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,WriteGPTTableExit);

    if (BlockIoHandleHardDrive != NULL)
    {
        /* Allocate and zero out buffer for primary GPT */
        Status = gBS->AllocatePool(EfiBootServicesData, 
                                   BlockIoHandleHardDrive->Media->BlockSize,
                                  (VOID*)&PartHdr );
        HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,WriteGPTTableExit);
        gBS->SetMem(PartHdr, BlockIoHandleHardDrive->Media->BlockSize, 0); 
        
        /* Read current primary GPT to get the GPT info */
        Status =  BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       PRIMARY_HEADER_LBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       PartHdr);    
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->ReadBlocks,WriteGPTTableExit);
        
        /* Allocate and zero out buffer for Backup GPT */
        Status = gBS->AllocatePool(EfiBootServicesData, 
                                   BlockIoHandleHardDrive->Media->BlockSize,
                                   (VOID*)&BackupPartHdr );
        HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,WriteGPTTableExit);
        gBS->SetMem(BackupPartHdr, BlockIoHandleHardDrive->Media->BlockSize, 0 ); 
                
        /* Read current backup GPT by given PartHdr->AlternateLBA*/
        Status =  BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       PartHdr->AlternateLBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       BackupPartHdr
                                                      );
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->ReadBlocks,WriteGPTTableExit);
        
        /* Assign the correct writing buffer and start LBA*/    
        if(TableType == BACKUP_GPT_TABLE ) 
        {   
            OutPartitionHeader = BackupPartHdr;
            OutStartLBA = PartHdr->LastUsableLBA + 1;         
        }
        else
        {   
            OutPartitionHeader = PartHdr;
            OutStartLBA = PartHdr->MyLBA + 1;        
        }

        StartPartitionEntry = (EFI_PARTITION_ENTRY*) *FileBuffer;  
        PartitionEntry = StartPartitionEntry;

        /*Calculate the PartitionEntryArrayCRC32 of the input GPT buffer*/   
        Status = gBS->CalculateCrc32 (
                        *FileBuffer, 
                        (OutPartitionHeader->NumberOfPartitionEntries * OutPartitionHeader->SizeOfPartitionEntry),
                        &Partition_Array_CRC32 );
        HANDLE_ERROR_LABEL(Status, gBS->CalculateCrc32, WriteGPTTableExit);

        OutPartitionHeader->PartitionEntryArrayCRC32 = Partition_Array_CRC32; 

        //Clear out the CRC to calculate the new header CRC
        OutPartitionHeader->Header.CRC32 = 0;
        Status = gBS->CalculateCrc32 (OutPartitionHeader, 
                                      OutPartitionHeader->Header.HeaderSize, 
                                      &CRCforHeader );
        HANDLE_ERROR_LABEL(Status, gBS->CalculateCrc32, WriteGPTTableExit);

        OutPartitionHeader->Header.CRC32 = CRCforHeader;

        BlocksToWrite = ((PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry) / BlockIoHandleHardDrive->Media->BlockSize);
        BlocksToWriteRemainder = ((PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry) % BlockIoHandleHardDrive->Media->BlockSize);
        BlocksToWrite += (BlocksToWriteRemainder == 0) ? 0 : 1;

        // Allocate and zero out buffer for blocks to be written 
        Status = gBS->AllocatePool(EfiBootServicesData, BlocksToWrite * BlockIoHandleHardDrive->Media->BlockSize, (VOID*)&BlocksToWriteBuffer);
        HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, WriteGPTTableExit);
        gBS->SetMem(BlocksToWriteBuffer, BlocksToWrite * BlockIoHandleHardDrive->Media->BlockSize, 0);
        if ( *FileSize <= (BlocksToWrite * BlockIoHandleHardDrive->Media->BlockSize))
        {
            gBS->CopyMem(BlocksToWriteBuffer, PartitionEntry, *FileSize);
        }
        else
        {
            Status = EFI_BAD_BUFFER_SIZE;
            HANDLE_ERROR_LABEL(Status, gBS->CopyMem, WriteGPTTableExit);
        }
        BlocksToWriteTempBuffer = BlocksToWriteBuffer;

        //Start to write all the partition entries
        for(CurrentLBA = OutStartLBA; CurrentLBA < OutStartLBA + BlocksToWrite; CurrentLBA++) 
        {
            Status = BlockIoHandleHardDrive->WriteBlocks ( BlockIoHandleHardDrive,
                                                           BlockIoHandleHardDrive->Media->MediaId,
                                                           CurrentLBA,
                                                           BlockIoHandleHardDrive->Media->BlockSize,
                                                           BlocksToWriteTempBuffer
                                                          );
            HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->WriteBlocks, WriteGPTTableExit);

           BlocksToWriteTempBuffer += BlockIoHandleHardDrive->Media->BlockSize;
        }

        // Write the Calculated CRC to the partition Header
        Status = BlockIoHandleHardDrive->WriteBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       OutPartitionHeader->MyLBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       OutPartitionHeader
                                                      );
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->WriteBlocks, WriteGPTTableExit);
        
        /* Flush the BlockIOHandle */
        Status = BlockIoHandleHardDrive->FlushBlocks (BlockIoHandleHardDrive);
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->FlushBlocks, WriteGPTTableExit);
    }
    else
    {
        Status = EFI_NOT_FOUND;
        HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,WriteGPTTableExit);
    }
    
WriteGPTTableExit:
    
    if (BackupPartHdr != NULL)
    {
        gBS->FreePool(BackupPartHdr);
        BackupPartHdr = NULL;
    }
    if (PartHdr != NULL)
    {
        gBS->FreePool(PartHdr);
        PartHdr = NULL;
    }

    if (BlocksToWriteBuffer != NULL)
    {
        gBS->FreePool(BlocksToWriteBuffer);
        BlocksToWriteBuffer = NULL;
    }

    return  Status;
}

/**
 * This function will retrive active GPT data from GPT table
 * 
 * @param EFI_GUID              :  Vendor GUID whose GPT has to be changed
 * @param GPT_TABLE_TYPE        :  Primary or backup GPT table
 * @param FW_LINKED_LIST_TYPE   :  Firmware entry list 
 * @param ACTIVE_GPT_SUB_DATA   :  Pointer to hold active GPT data
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
GetActiveGPTDataFromGPT (
    IN  EFI_GUID                      Memory_GUID,
    IN  GPT_TABLE_TYPE                TableType,
    IN  FW_LINKED_LIST_TYPE          *List,
    IN OUT ACTIVE_GPT_SUB_DATA       *GPTSubDataPtr
)
{
    EFI_STATUS                    Status                 = EFI_SUCCESS;
    EFI_PARTITION_ENTRY          *StartPartitionEntry    = NULL;
    EFI_PARTITION_ENTRY          *PartitionEntry         = NULL;
    UINT32                        NumberOfPartitions     = 0;
    UINT32                        SizeOfPartitionEntry   = 0;
    UINT32                        PartitionIndex         = 0;
    UINTN                         FileSize               = 0;
    FW_LIST_NODE_TYPE            *CurrentNode            = NULL;
    QPAYLOAD_FW_ENTRY            *FwEntry                = NULL;
    FWENTRY_DISK_PARTITION_TYPE   DiskPartType;
    UINTN                         i                      = 0;

    if((NULL == List)||
       (NULL == GPTSubDataPtr))
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetDiskPartAndGptNvType(
        Memory_GUID,
        NULL,
        &DiskPartType
        );
    HANDLE_ERROR_LABEL(Status,GetDiskPartAndGptNvType,GetActiveGPTDataFromGPTExit);
    

    Status = ReadGPTTable(Memory_GUID,
                          (VOID*)&StartPartitionEntry, 
                          &FileSize, 
                          &SizeOfPartitionEntry,
                          TableType);
    HANDLE_ERROR_LABEL(Status,ReadGPTTable (Memory_GUID),GetActiveGPTDataFromGPTExit);

    PartitionEntry = StartPartitionEntry;
    NumberOfPartitions = FileSize / SizeOfPartitionEntry;

    /* Go through each partition entry
     for loop 1:
        go through each partition entry by jumping partition etnry size at a time
     for loop 2:
        go through the partition entry info in the List to see if any match.
    */
    for(PartitionIndex = 0; PartitionIndex < NumberOfPartitions; PartitionIndex++) 
    {
        CurrentNode = List->Head;
        for( i = 0; i < List->Count; i++) 
        {
            FwEntry = (QPAYLOAD_FW_ENTRY*)(CurrentNode->Data);
            if(FwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION &&
               FwEntry->Metadata.BackupType == FWENTRY_UPDATE_TYPE_PARTITION &&
               FwEntry->Metadata.UpdatePath.DiskPartitionType == DiskPartType &&
               FwEntry->Metadata.BackupPath.DiskPartitionType == DiskPartType)
            {
                if(CompareGuid(&(StartPartitionEntry->PartitionTypeGUID), &(FwEntry->Metadata.UpdatePath.PartitionTypeGUID)) &&
                   CompareMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.UpdatePath.PartitionName, StrSize(StartPartitionEntry->PartitionName)) == 0)
                {
                    if(PartitionIndex > GPTSubDataPtr->MaxEntries)
                    {
                        Status = EFI_OUT_OF_RESOURCES;
                        DEBUG ((EFI_D_ERROR, "Error: PartitionIndex exceededs the maximum allowed GPT entries.\n"));
                        goto GetActiveGPTDataFromGPTExit;
                    }

                    CopyMem(&(GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].PartitionTypeGUID), &FwEntry->Metadata.UpdatePath.PartitionTypeGUID, sizeof (EFI_GUID));
                    CopyMem(GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].PartitionName, FwEntry->Metadata.UpdatePath.PartitionName, StrSize(FwEntry->Metadata.UpdatePath.PartitionName));

                    GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].TableIndex = PartitionIndex;
                    GPTSubDataPtr->UsedEntries++;

                    break;
                }
                else if(CompareGuid(&(StartPartitionEntry->PartitionTypeGUID), &(FwEntry->Metadata.BackupPath.PartitionTypeGUID)) &&
                        CompareMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.BackupPath.PartitionName, StrSize(StartPartitionEntry->PartitionName)) == 0)
                {
                    if(PartitionIndex > GPTSubDataPtr->MaxEntries)
                    {
                        Status = EFI_OUT_OF_RESOURCES;
                        DEBUG ((EFI_D_ERROR, "Error: PartitionIndex exceededs the maximum allowed GPT entries.\n"));
                        goto GetActiveGPTDataFromGPTExit;
                    }

                    CopyMem(&(GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].PartitionTypeGUID), &FwEntry->Metadata.BackupPath.PartitionTypeGUID, sizeof (EFI_GUID));
                    CopyMem(GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].PartitionName, FwEntry->Metadata.BackupPath.PartitionName, StrSize(FwEntry->Metadata.BackupPath.PartitionName));

                    GPTSubDataPtr->ActiveGPTEntrySubData[GPTSubDataPtr->UsedEntries].TableIndex = PartitionIndex;
                    GPTSubDataPtr->UsedEntries++;

                    break;
                }
            }
            CurrentNode = CurrentNode->Next;
        }
        StartPartitionEntry = (EFI_PARTITION_ENTRY *)((UINT8 *) StartPartitionEntry + SizeOfPartitionEntry);
    }

GetActiveGPTDataFromGPTExit:

    /* Free the buffer allocated in ReadGPTTable function*/
    if (PartitionEntry != NULL)
    {
        gBS->FreePool(PartitionEntry);
        PartitionEntry = NULL;
    }

    return Status;
}

/**
 * This function will retrive active GPT data from GPT table
 * 
 * @param EFI_GUID              :  Vendor GUID whose GPT has to be changed
 * @param GPT_TABLE_TYPE        :  Primary or backup GPT table
 * @param ACTIVE_GPT_SUB_DATA   :  Pointer to hold active GPT data
 * @param PartitionEntry        :  Partition table entry 
 * @param GPTFileSize    :  Size of PartitionEntry 
 * 
 * gBS->FreePool will be used to free OUT buffer
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
GetGPTFromActiveGPTData (
    IN  EFI_GUID                      Memory_GUID,
    IN  GPT_TABLE_TYPE                TableType,
    IN  ACTIVE_GPT_SUB_DATA          *GPTSubDataPtr,
    OUT VOID                        **PartitionEntry,
    OUT UINTN                        *GPTFileSize,
    OUT OPTIONAL UINT32              *PartitionSize
)
{
    EFI_STATUS                    Status                 = EFI_SUCCESS;
    EFI_PARTITION_ENTRY          *StartPartitionEntry    = NULL;
    UINT32                        SizeOfPartitionEntry   = 0;
    UINT32                        NumberOfPartitions     = 0;
    UINT32                        PartitionIndex         = 0;
    UINTN                         GPTSize                = 0;
    UINTN                         i                      = 0;

    if(NULL == GPTSubDataPtr)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = ReadGPTTable(Memory_GUID,
                          (VOID*)&StartPartitionEntry, 
                          &GPTSize, 
                          &SizeOfPartitionEntry,
                          TableType);
    HANDLE_ERROR_LABEL(Status,ReadGPTTable (Memory_GUID),GetGPTFromActiveGPTDataExit);

    NumberOfPartitions = GPTSize / SizeOfPartitionEntry;

    if(NULL != PartitionSize)
    {
        *PartitionSize = SizeOfPartitionEntry;
    }

    *PartitionEntry = StartPartitionEntry;
    *GPTFileSize = GPTSize;

    /* Go through each partition entry
     for loop 1:
        go through each partition entry by jumping partition etnry size at a time
     for loop 2:
        go through the GPT backup sub data to see if the table index match with current patition index.
    */
    for(PartitionIndex = 0; PartitionIndex < NumberOfPartitions; PartitionIndex++) 
    {
        for(i = 0; i < GPTSubDataPtr->UsedEntries ; i++) 
        {
            if(PartitionIndex == GPTSubDataPtr->ActiveGPTEntrySubData[i].TableIndex)
            {
                StartPartitionEntry->PartitionTypeGUID = GPTSubDataPtr->ActiveGPTEntrySubData[i].PartitionTypeGUID;
                CopyMem(
                    StartPartitionEntry->PartitionName,
                    GPTSubDataPtr->ActiveGPTEntrySubData[i].PartitionName,
                    sizeof(GPTSubDataPtr->ActiveGPTEntrySubData[i].PartitionName)
                    );
                break;
            }
        }
        StartPartitionEntry = (EFI_PARTITION_ENTRY *)((UINT8 *)StartPartitionEntry + SizeOfPartitionEntry);
    }

GetGPTFromActiveGPTDataExit:

    return Status;
}

/**
 * This function will retrive active GPT data from GPT table
 * 
 * @param EFI_GUID                      :  Vendor GUID whose GPT has to be changed
 * @param QCOM_FW_UPDATE_NV_TYPE        :  GPTNvType name to be return
 * @param FWENTRY_DISK_PARTITION_TYPE   :  Type of disk partition to be filled
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
GetDiskPartAndGptNvType(
    IN EFI_GUID                                   Memory_GUID,
    OUT OPTIONAL QCOM_FW_UPDATE_NV_TYPE          *GPTNvTypePtr,
    OUT OPTIONAL FWENTRY_DISK_PARTITION_TYPE     *DiskPartTypePtr
    )
{
    EFI_STATUS      Status                 = EFI_SUCCESS;
    EFI_GUID        Emmc_BOOT_GUID         = gEfiEmmcBootPartition1Guid;
    EFI_GUID        Emmc_GPP_GUID          = gEfiEmmcGppPartition1Guid;
    EFI_GUID        UFS_BOOT_LUN1_GUID     = gEfiUfsLU1Guid;
    EFI_GUID        UFS_BOOT_LUN2_GUID     = gEfiUfsLU2Guid;
    EFI_GUID        UFS_LUN4_GUID          = gEfiUfsLU4Guid;
    EFI_GUID        SPINOR_GUID            = gEfiSpiNor0Guid;
    EFI_GUID        NVME_GUID              = gEfiNvme0Guid;
    QCOM_FW_UPDATE_NV_TYPE GPTNvType       = QCOM_FW_UPDATE_NV_MAX;         
    FWENTRY_DISK_PARTITION_TYPE DiskPartType = FWENTRY_DISK_PARTITION_MAX;

    // Return error if both Optional parameters are missing
    if ((NULL == DiskPartTypePtr) && 
        (NULL == GPTNvTypePtr))
    {
        Status = EFI_INVALID_PARAMETER;
        goto GetDiskPartAndGptNvNameExit;
    }

    if (CompareGuid(&Memory_GUID, &Emmc_BOOT_GUID))
    {
        DiskPartType = FWENTRY_EMMC_PARTITION_BOOT1;
        GPTNvType = QCOM_FW_UPDATE_NV_BP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &Emmc_GPP_GUID))
    {
        DiskPartType = FWENTRY_EMMC_PARTITION_GPP1;
        GPTNvType = QCOM_FW_UPDATE_NV_GPP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &UFS_BOOT_LUN1_GUID))
    {
        DiskPartType = FWENTRY_UFS_LUN1;
        GPTNvType = QCOM_FW_UPDATE_NV_BP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &UFS_BOOT_LUN2_GUID))
    {
        DiskPartType = FWENTRY_UFS_LUN2;
        GPTNvType = QCOM_FW_UPDATE_NV_BP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &UFS_LUN4_GUID))
    {
        DiskPartType = FWENTRY_UFS_LUN4;
        GPTNvType = QCOM_FW_UPDATE_NV_GPP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &SPINOR_GUID))
    {
        DiskPartType = FWENTRY_SPINOR;
        GPTNvType = QCOM_FW_UPDATE_NV_BP_GPT;
    }
    else if (CompareGuid(&Memory_GUID, &NVME_GUID))
    {
        DiskPartType = FWENTRY_NVME;
        GPTNvType = QCOM_FW_UPDATE_NV_GPP_GPT;
    }
    else
    {
        Status = EFI_INVALID_PARAMETER;
        goto GetDiskPartAndGptNvNameExit;
    }

    if(NULL != DiskPartTypePtr)
    {
        *DiskPartTypePtr = DiskPartType;
    }
    
    if(NULL != GPTNvTypePtr)
    {
        *GPTNvTypePtr = GPTNvType;
    }

GetDiskPartAndGptNvNameExit:

    return Status;
}

/**
 * This function serves 2 purposes based on input parameter 
 * 'SwitchGpt" : 
 *  
 * If SwitchGpt = TRUE, it will switch GUID with backup and vice
 * versa as requested by parsing the input firmware entry list, 
 * and write the Buffer into the specified GPT Table. It will 
 * also calculate the CRC's and rewrite the header for the 
 * modified table 
 *  
 * If SwitchGpt = FALSE, it will go through the input firmware 
 * entry list and check if GPT entry switch is needed for the 
 * given Memory_GUID 
 * 
 * @param Memory_GUID :  Vendor GUID whose GPT has to be changed
 * @param List        :  Firmware entry list 
 * @param SwitchGpt   :  If TRUE, switch GPT entries if 
 *                       needed
 *                       If FALSE, return if GPT switch is
 *                       needed
 * @param NeedSwitchGpt: If SwitchGpt is FALSE, return if GPT 
 *                     switch is needed
 *                       If SwitchGpt is TRUE, ignored 
 * @return EFI_STATUS 
 */

EFI_STATUS 
SwitchBackupGptTableEntries(
    IN EFI_GUID             Memory_GUID,
    IN FW_LINKED_LIST_TYPE *List,
    IN BOOLEAN              SwitchGpt,
    OUT BOOLEAN            *NeedSwitchGpt
    )
{
    EFI_STATUS                    Status                 = EFI_SUCCESS;
    EFI_PARTITION_ENTRY          *StartPartitionEntry    = NULL;
    EFI_PARTITION_ENTRY          *PartitionEntry         = NULL;
    UINT32                        NumberOfPartitions     = 0;
    UINT32                        PartitionIndex         = 0;
    UINT32                        SizeOfPartitionEntry   = 0;
    EFI_BLOCK_IO_PROTOCOL        *BlockIoHandleHardDrive = NULL;
    UINTN                         FileSize               = 0;
    FW_LIST_NODE_TYPE            *CurrentNode            = NULL;
    QPAYLOAD_FW_ENTRY            *FwEntry                = NULL;
    FWENTRY_DISK_PARTITION_TYPE   DiskPartType           = FWENTRY_DISK_PARTITION_MAX;
    BOOLEAN                       MatchedEntryFound      = FALSE;
    UINTN                         i                      = 0;
    QCOM_FW_UPDATE_NV_TYPE        GPTNvType              = QCOM_FW_UPDATE_NV_MAX;
    ACTIVE_GPT_SUB_DATA          *GPTSubDataPtr          = NULL;
    UINT32                        GPTSubDataSize         = 0;
    COMPRESSED_DATA              *CompressedDataPtr      = NULL;
    UINTN                         CompressedDataSize     = 0;

    /* Check input parameters, 
       1. It supports Boot and GPP EMMC type and LUN1, LUN2 and LUN4 UFS type
       2. List shouldn't be NULL
       3. if List-> Count == 0, return SUCCESS
       4. List -> Head == NULL is not allowed
       5. when SwitchGpt = FALSE, NeedSwitchGpt can not be NULL
       6. When Memory_GUID = LUN1 or LUN2, SwitchGpt = TRUE is not allowed
       7. When Memory_GUID = LUN1 or LUN2 and when SwitchGpt = FALSE, return NeedSwitchGpt = FALSE
     */

    /* Check 1 */
    Status = GetDiskPartAndGptNvType(
        Memory_GUID,
        &GPTNvType,
        &DiskPartType
        );
    HANDLE_ERROR_LABEL(Status,GetDiskPartAndGptNvType,SwitchBackupGptTableEntriesExit);

    if ((DiskPartType == FWENTRY_UFS_LUN1) ||
        (DiskPartType == FWENTRY_UFS_LUN2))
    {
        //Check 6
        if(SwitchGpt == TRUE)
        {
            DEBUG((EFI_D_WARN,"\n    Skipping backup of GPT table for LUN1/LUN2\n"));
        }
        else //Check 7
        {
            NeedSwitchGpt = FALSE;
        }
        return Status;
    }

    /* Check 2 */
    if (List == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }
    /* Check 3 */
    if (List->Count == 0)  
    {
        return EFI_SUCCESS;
    }

    /* Check 4 */
    if (List->Head == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }
      
    /* Check 5 */
    if ((SwitchGpt == FALSE) &&
        (NeedSwitchGpt == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    // Locate Disk type BlockIO handler
    Status = GetDiskTypeHandle(Memory_GUID, &BlockIoHandleHardDrive);
    HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,SwitchBackupGptTableEntriesExit);

    if ( BlockIoHandleHardDrive != NULL)
    {
        // Read partition entries from backup      
        Status = GetGPTNvData(GPTNvType, (VOID **)&CompressedDataPtr, &CompressedDataSize);       
        HANDLE_ERROR_LABEL(Status,GetGPTNvData,SwitchBackupGptTableEntriesExit);

        GPTSubDataSize = CompressedDataPtr->UncompressedSize;
        GPTSubDataPtr = AllocateZeroPool (GPTSubDataSize);
        Status = (NULL == GPTSubDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
        HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GPTSubDataPtr),  SwitchBackupGptTableEntriesExit);

        Status = DecompressGPT(
            CompressedDataPtr->Data,
            (CompressedDataPtr->Size - OFFSET_OF(COMPRESSED_DATA, Data)),
            GPTSubDataPtr,
            &GPTSubDataSize);
        HANDLE_ERROR_LABEL(Status,DecompressGPT,SwitchBackupGptTableEntriesExit);

        // Derive the original GPT table using GPT sub data
        Status = GetGPTFromActiveGPTData (
            Memory_GUID,
            BACKUP_GPT_TABLE,
            GPTSubDataPtr,
            (VOID*)&PartitionEntry,
            &FileSize,
            &SizeOfPartitionEntry
            );
        HANDLE_ERROR_LABEL(Status,GetGPTFromActiveGPTData,SwitchBackupGptTableEntriesExit);

        StartPartitionEntry = PartitionEntry;

        NumberOfPartitions = FileSize / SizeOfPartitionEntry;

        /* Go through each partition entry
         for loop 1:
            go through each partition entry by jumping partition etnry size at a time
         for loop 2:
            go through the partition entry info in the List to see if any match.
        */
        for(PartitionIndex = 0; PartitionIndex < NumberOfPartitions; PartitionIndex++) 
        {
            CurrentNode = List->Head;
            for( i = 0; i < List->Count; i++) 
            {
                FwEntry = (QPAYLOAD_FW_ENTRY*)(CurrentNode->Data);
                if(FwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION &&
                   FwEntry->Metadata.BackupType == FWENTRY_UPDATE_TYPE_PARTITION &&
                   FwEntry->Metadata.UpdatePath.DiskPartitionType == DiskPartType &&
                   FwEntry->Metadata.BackupPath.DiskPartitionType == DiskPartType)
                {
                    if(CompareGuid(&(StartPartitionEntry->PartitionTypeGUID), &(FwEntry->Metadata.UpdatePath.PartitionTypeGUID)) &&
                       CompareMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.UpdatePath.PartitionName, StrSize(StartPartitionEntry->PartitionName)) == 0)
                    {
                        MatchedEntryFound = TRUE;
                        StartPartitionEntry->PartitionTypeGUID = FwEntry->Metadata.BackupPath.PartitionTypeGUID;
                        CopyMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.BackupPath.PartitionName, sizeof(StartPartitionEntry->PartitionName));
                        break;
                    }
                    else if(CompareGuid(&(StartPartitionEntry->PartitionTypeGUID), &(FwEntry->Metadata.BackupPath.PartitionTypeGUID)) &&
                            CompareMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.BackupPath.PartitionName, StrSize(StartPartitionEntry->PartitionName)) == 0)
                    {
                        MatchedEntryFound = TRUE;
                        StartPartitionEntry->PartitionTypeGUID = FwEntry->Metadata.UpdatePath.PartitionTypeGUID;
                        CopyMem(StartPartitionEntry->PartitionName, FwEntry->Metadata.UpdatePath.PartitionName, sizeof(StartPartitionEntry->PartitionName));
                        break;
                    }
                }
                CurrentNode = CurrentNode->Next;
            }
            StartPartitionEntry = (EFI_PARTITION_ENTRY *)((UINT8 *) StartPartitionEntry + SizeOfPartitionEntry);
        }

        if(SwitchGpt == FALSE) 
        {
            *NeedSwitchGpt = MatchedEntryFound;
            Status = EFI_SUCCESS;
            goto SwitchBackupGptTableEntriesExit;
        }

        Status = WriteGPTTable((void **)&PartitionEntry, &FileSize, Memory_GUID, BACKUP_GPT_TABLE);
        HANDLE_ERROR_LABEL(Status, WriteGPTTable, SwitchBackupGptTableEntriesExit);
    }
    else
    {
        Status = EFI_NOT_FOUND;
        HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,SwitchBackupGptTableEntriesExit);
    }

SwitchBackupGptTableEntriesExit:

    //Free all the Allocated Memory
    if(PartitionEntry != NULL) 
    {
        gBS->FreePool(PartitionEntry);
        PartitionEntry = NULL;
    }

    if (NULL != GPTSubDataPtr)
    {
        FreePool (GPTSubDataPtr);
        GPTSubDataPtr = NULL;
    }

    if (NULL != CompressedDataPtr)
    {
        FreePool (CompressedDataPtr);
        CompressedDataPtr = NULL;
    }

    return Status;
}

/**
 * Modifies Boot Partition and GPP partition tables to point to the backup tables 
 * Also modifies and fills in the right CRC 
 * 
 * @param TABLE_TYPE
 * 
 * @param RestoreBootGpt 
 *  
 * @param RestoreGppGpt  
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS 
RestoreAllGPTTablesFromDPP(
    IN GPT_TABLE_TYPE TableType,
    IN BOOLEAN RestoreBootGpt,
    IN BOOLEAN RestoreGppGpt
    )
{

    EFI_STATUS              Status                  = EFI_SUCCESS;
    UINTN                   GPTFileSize             = 0;
    VOID                   *BootPartitionEntry      = NULL;
    VOID                   *GppPartitionEntry       = NULL;
    EFI_GUID                BOOT_GUID;
    EFI_GUID                GPP_GUID;
    ACTIVE_GPT_SUB_DATA    *BootGPTSubDataPtr       = NULL;
    ACTIVE_GPT_SUB_DATA    *GppGPTSubDataPtr        = NULL;
    UINT32                  BootGPTSubDataSize      = 0;
    UINT32                  GppGPTSubDataSize       = 0;
    COMPRESSED_DATA        *CompressedDataPtr    = NULL;
    UINTN                   CompressedDataSize   = 0;

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, RestoreAllGPTTablesFromDPPExit);
   
    if(RestoreBootGpt) 
    {
        Status = GetGPTNvData(QCOM_FW_UPDATE_NV_BP_GPT, (VOID **)&CompressedDataPtr, &CompressedDataSize);       
        HANDLE_ERROR_LABEL(Status,GetGPTNvData,RestoreAllGPTTablesFromDPPExit);

        BootGPTSubDataSize = CompressedDataPtr->UncompressedSize;
        BootGPTSubDataPtr = AllocateZeroPool (BootGPTSubDataSize);
        Status = (NULL == BootGPTSubDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
        HANDLE_ERROR_LABEL(Status, AllocateZeroPool (BootGPTSubDataPtr),  RestoreAllGPTTablesFromDPPExit);

        Status = DecompressGPT(
            CompressedDataPtr->Data,
            (CompressedDataPtr->Size - OFFSET_OF(COMPRESSED_DATA, Data)),
            BootGPTSubDataPtr,
            &BootGPTSubDataSize);
        HANDLE_ERROR_LABEL(Status,DecompressGPT,RestoreAllGPTTablesFromDPPExit);

        // Derive the original GPT table using GPT sub data
        Status = GetGPTFromActiveGPTData (
            BOOT_GUID,
            TableType,
            BootGPTSubDataPtr,
            &BootPartitionEntry,
            &GPTFileSize,
            NULL
            );
        HANDLE_ERROR_LABEL(Status,GetGPTFromActiveGPTData,RestoreAllGPTTablesFromDPPExit);

        Status = WriteGPTTable(&BootPartitionEntry, &GPTFileSize, BOOT_GUID, TableType);
        HANDLE_ERROR_LABEL(Status,WriteGPTTable,RestoreAllGPTTablesFromDPPExit);

        FreePool (CompressedDataPtr);
        CompressedDataPtr = NULL;
    }

    if(RestoreGppGpt) 
    {
        Status = GetGPTNvData(QCOM_FW_UPDATE_NV_GPP_GPT, (VOID **)&CompressedDataPtr, &CompressedDataSize);       
        HANDLE_ERROR_LABEL(Status,GetGPTNvData,RestoreAllGPTTablesFromDPPExit);

        GppGPTSubDataSize = CompressedDataPtr->UncompressedSize;
        GppGPTSubDataPtr = AllocateZeroPool (GppGPTSubDataSize);
        Status = (NULL == GppGPTSubDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
        HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GppGPTSubDataPtr),  RestoreAllGPTTablesFromDPPExit);

        Status = DecompressGPT(
            CompressedDataPtr->Data,
            (CompressedDataPtr->Size - OFFSET_OF(COMPRESSED_DATA, Data)),
            GppGPTSubDataPtr,
            &GppGPTSubDataSize);
        HANDLE_ERROR_LABEL(Status,DecompressGPT,RestoreAllGPTTablesFromDPPExit);

        // Derive the original GPT table using GPT sub data
        Status = GetGPTFromActiveGPTData (
            GPP_GUID,
            TableType,
            GppGPTSubDataPtr,
            &GppPartitionEntry,
            &GPTFileSize,
            NULL
            );
        HANDLE_ERROR_LABEL(Status,GetGPTFromActiveGPTData,RestoreAllGPTTablesFromDPPExit);

        Status = WriteGPTTable(&GppPartitionEntry, &GPTFileSize, GPP_GUID ,TableType);
        HANDLE_ERROR_LABEL(Status,WriteGPTTable,RestoreAllGPTTablesFromDPPExit);

        FreePool (CompressedDataPtr);
        CompressedDataPtr = NULL;
    }
      
RestoreAllGPTTablesFromDPPExit:

    if(BootPartitionEntry != NULL)
    {
        gBS->FreePool(BootPartitionEntry);
        BootPartitionEntry = NULL;
    }
    if(GppPartitionEntry != NULL)
    {
        gBS->FreePool(GppPartitionEntry);
        GppPartitionEntry = NULL;
    }

    if (BootGPTSubDataPtr != NULL)
    {
        FreePool(BootGPTSubDataPtr);
        BootGPTSubDataPtr = NULL;
    }

    if (GppGPTSubDataPtr != NULL)
    {
        FreePool(GppGPTSubDataPtr);
        GppGPTSubDataPtr = NULL;
    }

    if (NULL != CompressedDataPtr)
    {
        FreePool (CompressedDataPtr);
        CompressedDataPtr = NULL;
    }

    return Status;
}

/**
 * Modifies gpt partition tables (BOOT partition and GPP) based 
 * on firmware entries in payload. 
 *
 * @param List - Firmware entry List 
 * @param ModifyBootGpt - Indicate whether or not modify BOOT 
 *                      GPT
 * @param ModifyGppGpt - Indicate whether or not modify GPP GPT 
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS 
ModifyAllBackupGptTablesForFailSafeUpdate(
    IN  FW_LINKED_LIST_TYPE *List,
    IN  BOOLEAN              ModifyBootGpt,
    IN  BOOLEAN              ModifyGppGpt
    )
{
    EFI_STATUS Status  = EFI_SUCCESS;
    EFI_GUID BOOT_GUID;
    EFI_GUID GPP_GUID;
    
    // Check input parameters
    if(List->Count == 0 ||
       List->Head == NULL) 
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, ModifyAllBackupGptTablesForFailSafeUpdateExit);

    if (ModifyBootGpt) 
    {
        // Switch backup GPT table entries for BOOT
        Status = SwitchBackupGptTableEntries(BOOT_GUID, List, TRUE, NULL);
        if(EFI_ERROR(Status)) 
        {
            return Status;
        }
    }

    if(ModifyGppGpt) 
    {
        // Switch backup GPT table entries for GPP
        Status = SwitchBackupGptTableEntries(GPP_GUID, List, TRUE, NULL);
    }

ModifyAllBackupGptTablesForFailSafeUpdateExit:

    return Status;
}

/**
 * Invalidate ALL Primary GPT Tables i.e BOOT and GPP
 * Partitions
 *
 * @param InvalidateBootGpt - Indicate whether or not invalidate 
 *                          BOOT partition GPT
 * @param InvalidateGppGpt - Indicate whether or not invalidate 
 *                         GPP partition GPT
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS 
InvalidateAllPrimaryGPT(
  IN BOOLEAN InvalidateBootGpt,
  IN BOOLEAN InvalidateGppGpt
  )
{
    EFI_STATUS Status    = EFI_SUCCESS;
    EFI_GUID   BOOT_GUID;
    EFI_GUID   GPP_GUID;

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, InvalidateAllPrimaryGPTExit);

    if(InvalidateBootGpt) 
    {
        Status = InvalidatePrimaryGPT(BOOT_GUID);
        if(EFI_ERROR(Status))
        {   
            return Status;
        }
    }

    if(InvalidateGppGpt) 
    {
        Status = InvalidatePrimaryGPT(GPP_GUID);
    }

InvalidateAllPrimaryGPTExit:

    return Status;
}

/**
 * Invalidate ALL Backup GPT Tables
 *
 * @param InvalidateBootGpt - Indicate whether or not invalidate
 *                          BOOT partition GPT
 * @param InvalidateGppGpt - Indicate whether or not invalidate 
 *                         GPP partition GPT
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS 
InvalidateAllBackUpGPT(
    IN BOOLEAN InvalidateBootGpt,
    IN BOOLEAN InvalidateGppGpt
    )
{

    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID BOOT_GUID;
    EFI_GUID GPP_GUID;

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, InvalidateAllBackUpGPTExit);

    if(InvalidateBootGpt) 
    {
        Status = InvalidateBackUpGPT(BOOT_GUID);
        if(EFI_ERROR(Status))
        {   
            return Status;
        }
    }
        
    if(InvalidateGppGpt) 
    {
        Status = InvalidateBackUpGPT(GPP_GUID);
    }
    
InvalidateAllBackUpGPTExit:

    return Status;
    
}

/**
 * Invalidate the Primary GPT Table
 *
 * @param Memory_GUID
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS 
InvalidatePrimaryGPT(
    IN EFI_GUID Memory_GUID
    )
{    
    EFI_STATUS                   Status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL       *BlockIoHandleHardDrive = NULL;
    EFI_PARTITION_TABLE_HEADER  *PartHdr = NULL;
        
    Status = GetDiskTypeHandle(Memory_GUID,&BlockIoHandleHardDrive);
    HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,InvalidatePrimaryGPTExit);

    if (BlockIoHandleHardDrive != NULL)
    {
        Status = gBS->AllocatePool(EfiBootServicesData  , BlockIoHandleHardDrive->Media->BlockSize,(VOID*)&PartHdr );
        HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,InvalidatePrimaryGPTExit);
        gBS->SetMem(PartHdr, BlockIoHandleHardDrive->Media->BlockSize, 0 );
      
        Status =  BlockIoHandleHardDrive->ReadBlocks (BlockIoHandleHardDrive,
                                                      BlockIoHandleHardDrive->Media->MediaId,
                                                      PRIMARY_HEADER_LBA,
                                                      BlockIoHandleHardDrive->Media->BlockSize,
                                                      PartHdr
                                                      );
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->ReadBlocks, InvalidatePrimaryGPTExit);

        /* Invalidate the GPT by modify the CRC */
        PartHdr->Header.CRC32 = 0;
        
        Status = BlockIoHandleHardDrive->WriteBlocks(BlockIoHandleHardDrive,
                                                     BlockIoHandleHardDrive->Media->MediaId,
                                                     PRIMARY_HEADER_LBA,
                                                     BlockIoHandleHardDrive->Media->BlockSize,
                                                     PartHdr);
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->WriteBlocks, InvalidatePrimaryGPTExit);
      
        Status = BlockIoHandleHardDrive->FlushBlocks (BlockIoHandleHardDrive);
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->FlushBlocks, InvalidatePrimaryGPTExit);
    }
    else
    {
        Status = EFI_NOT_FOUND;
        HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,InvalidatePrimaryGPTExit);
    }
               
InvalidatePrimaryGPTExit:  

    if (PartHdr != NULL)
    {
        gBS->FreePool(PartHdr);
        PartHdr = NULL;
    }

    return Status;

 }
               

/**
 * Invalidate the Backup GPT Table
 *
 * @param Memory_GUID
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS 
InvalidateBackUpGPT(
    IN EFI_GUID Memory_GUID
    )
{
    EFI_STATUS             Status                 = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *BlockIoHandleHardDrive = NULL;
    EFI_PARTITION_TABLE_HEADER *PartHdr           = NULL;
    EFI_PARTITION_TABLE_HEADER *PartHdr1          = NULL;
        
    Status = GetDiskTypeHandle(Memory_GUID,&BlockIoHandleHardDrive);
    HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,InvalidateBackUpGPTExit);
  
    if (BlockIoHandleHardDrive != NULL) 
    {
        Status = gBS->AllocatePool(EfiBootServicesData  , BlockIoHandleHardDrive->Media->BlockSize,(VOID*)&PartHdr );
        HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,InvalidateBackUpGPTExit);
        gBS->SetMem(PartHdr, BlockIoHandleHardDrive->Media->BlockSize, 0 );
      
        Status = gBS->AllocatePool(EfiBootServicesData  , BlockIoHandleHardDrive->Media->BlockSize,(VOID*)&PartHdr1 );
        HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,InvalidateBackUpGPTExit);
        gBS->SetMem(PartHdr1, BlockIoHandleHardDrive->Media->BlockSize, 0 );
       
        Status =  BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       PRIMARY_HEADER_LBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       PartHdr
                                                       );
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->ReadBlocks,InvalidateBackUpGPTExit);
      
        Status =  BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       PartHdr->AlternateLBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       PartHdr1
                                                       );
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->ReadBlocks,InvalidateBackUpGPTExit);
      
        /* Invalidate the GPT by modify the CRC */
        PartHdr1->Header.CRC32 = 0;  
      
        Status = BlockIoHandleHardDrive->WriteBlocks(BlockIoHandleHardDrive,
                                                     BlockIoHandleHardDrive->Media->MediaId,
                                                     PartHdr->AlternateLBA,
                                                     BlockIoHandleHardDrive->Media->BlockSize,
                                                     PartHdr1);
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->WriteBlocks,InvalidateBackUpGPTExit);
      
        Status = BlockIoHandleHardDrive->FlushBlocks (BlockIoHandleHardDrive);
        HANDLE_ERROR_LABEL(Status,BlockIoHandleHardDrive->FlushBlocks,InvalidateBackUpGPTExit);
    }
    else
    {
        Status = EFI_NOT_FOUND;
        HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,InvalidateBackUpGPTExit);
    }              
InvalidateBackUpGPTExit:

    if (PartHdr != NULL)
    {
       gBS->FreePool(PartHdr);
       PartHdr = NULL;
    }
    if (PartHdr1 != NULL)
    {
       gBS->FreePool(PartHdr1);
       PartHdr1 = NULL;
    }

    return Status;

 }

/**
 * Reads the partition table and returns Filesize and FileBuffer
 * with data from the GPT tables. In case FileBuffer is NULL
 * will return only the FileSize by reading the GPT header
 *
 * @param Memory_GUID     : Vendor GUID for which GPT tables
 *                          have to be read
 * @param FileBuffer      : Pointer to buffer where GPT should
 *                          be read. If NULL, Function will
 *                          return only the FileSize
 * @param FileSize        : Size of the buffer to be read, i.e
 *                          size of the partition table
 * @param SizeOfPartitionEntry : Size of one partition entry in
 *                          the partition table
 * @param TableType
 * 
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS 
ReadGPTTable(
    IN  EFI_GUID         Memory_GUID,
    OUT OPTIONAL VOID  **FileBuffer,
    OUT UINTN           *FileSize,
    OUT OPTIONAL UINT32 *SizeOfPartitionEntry,
    IN  GPT_TABLE_TYPE   TableType
    )
{
    EFI_STATUS                  Status                 = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL      *BlockIoHandleHardDrive = NULL;
    EFI_PARTITION_TABLE_HEADER *PartHdr                = NULL;
    EFI_PARTITION_ENTRY        *PartitionEntry         = NULL;
    UINT8                       *BlocksReadBuffer      = NULL;
    UINT8                       *BlocksReadBufferTemp  = NULL;
    UINT32                      BlocksToRead           = 0;
    UINT32                      BlocksToReadRemainder    = 0;
    UINT32                      i                      = 0;
    /*Logical Block Address */
    UINT32                      StartLBA               = 0;  

    /* NULL FileBuffer is allowed */
    if(FileSize == NULL) 
    {
        return EFI_INVALID_PARAMETER;
    }
    
    /* Initialize the out put buffer pointer and size */
    *FileSize = 0;

    /* Get the handle of given Disk type Memory GUID */
    Status = GetDiskTypeHandle(Memory_GUID , &BlockIoHandleHardDrive);
    HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,ReadGPTTableExit);

    if (BlockIoHandleHardDrive != NULL)
    {
        Status = gBS->AllocatePool(EfiBootServicesData , BlockIoHandleHardDrive->Media->BlockSize,(VOID*)&PartHdr );
        HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, ReadGPTTableExit);
        gBS->SetMem(PartHdr, BlockIoHandleHardDrive->Media->BlockSize, 0 ); 
      
        /* Read the Disk partition Header */
        Status =  BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                       BlockIoHandleHardDrive->Media->MediaId,
                                                       PRIMARY_HEADER_LBA,
                                                       BlockIoHandleHardDrive->Media->BlockSize,
                                                       PartHdr
                                                       );    
        HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->ReadBlocks, ReadGPTTableExit);
      
        /* Get the Partition Table Size */
        *FileSize = (PartHdr->NumberOfPartitionEntries) * (PartHdr->SizeOfPartitionEntry);
      
        if (NULL != SizeOfPartitionEntry)
        {
            *SizeOfPartitionEntry = PartHdr->SizeOfPartitionEntry;
        }

        /* Return if FileSize is the only requestd data*/
        if (FileBuffer == NULL)
        {
            return EFI_SUCCESS;
        }
      
        /* Get the total number of blocks to be read */
        BlocksToRead = (PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry) / BlockIoHandleHardDrive->Media->BlockSize;
        BlocksToReadRemainder  = (PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry) % BlockIoHandleHardDrive->Media->BlockSize;
        BlocksToRead += (BlocksToReadRemainder == 0) ? 0 : 1;
      
        /* Allocate and zero out a buffer to store the blocks read from disk */
        Status = gBS->AllocatePool(EfiBootServicesData,
                                   BlocksToRead * BlockIoHandleHardDrive->Media->BlockSize,
                                   (VOID*)&BlocksReadBuffer);
        HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, ReadGPTTableExit);
        gBS->SetMem(BlocksReadBuffer, BlocksToRead * BlockIoHandleHardDrive->Media->BlockSize, 0);
      
        /* Allocate and zero out a buffer to store the partition table */
        Status = gBS->AllocatePool(EfiBootServicesData, 
                                   (PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry),
                                   (VOID*)&PartitionEntry);
        HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, ReadGPTTableExit);
        gBS->SetMem(PartitionEntry , (PartHdr->NumberOfPartitionEntries * PartHdr->SizeOfPartitionEntry), 0 );
      
        /* The output buffer points to the first partition entry */
        *FileBuffer = (VOID*)PartitionEntry;
      
        /* Depends on reading primary or back up GPT, Assign the correct startLBA 
           by given TableType 
        */
        if(TableType == PRIMARY_GPT_TABLE) 
        {
            StartLBA = PartHdr->MyLBA + 1;
        }
        else
        {
            StartLBA = PartHdr->LastUsableLBA + 1;
        }
      
        BlocksReadBufferTemp = BlocksReadBuffer;
      
        // Start reading all the partitions one block at a time
        for (i = StartLBA; i < (StartLBA + BlocksToRead); i++)
        {
            Status = BlockIoHandleHardDrive->ReadBlocks ( BlockIoHandleHardDrive,
                                                          BlockIoHandleHardDrive->Media->MediaId,
                                                          i,
                                                          BlockIoHandleHardDrive->Media->BlockSize,
                                                          BlocksReadBufferTemp);
            HANDLE_ERROR_LABEL(Status, BlockIoHandleHardDrive->ReadBlocks, ReadGPTTableExit);
      
            BlocksReadBufferTemp += BlockIoHandleHardDrive->Media->BlockSize;
        }
      
        gBS->CopyMem(PartitionEntry, BlocksReadBuffer, (PartHdr->NumberOfPartitionEntries) * (PartHdr->SizeOfPartitionEntry));

        if (BlocksReadBuffer != NULL)
        {
            gBS->FreePool(BlocksReadBuffer);
            BlocksReadBuffer = NULL;
        }
      
        gBS->FreePool(PartHdr);
        PartHdr = NULL;
      
        return EFI_SUCCESS;
    }
    else
    {
        Status = EFI_NOT_FOUND;
        HANDLE_ERROR_LABEL(Status,GetDiskTypeHandle,ReadGPTTableExit);
    }

ReadGPTTableExit:

    if (PartHdr != NULL)
    {
        gBS->FreePool(PartHdr);
        PartHdr = NULL;
    }
    if(FileBuffer != NULL)
    {
      if (*FileBuffer != NULL)
      {
          gBS->FreePool(*FileBuffer);
          *FileBuffer = NULL;
      }
    }
    if (BlocksReadBuffer != NULL)
    {
        gBS->FreePool(BlocksReadBuffer);
        BlocksReadBuffer = NULL;
    }
    return Status;
}

/**
 * Copy GUID Partition Table from Boot partition and GPP Partition Table to FAT16.bin in files BPT.TBL GPP.TBL
 *
 * @param BackupBootGpt : Indicate whether or not backup BOOT 
 *                      partition GPT
 * @param BackupGppGpt  : Indicate whether or not backup GPP 
 *                      partition GPT
 *
 * @return EFI _STATUS
 *
 */

EFI_STATUS 
BackupGPTTable(
   IN BOOLEAN BackupBootGpt,
   IN BOOLEAN BackupGppGpt
   )
{

    EFI_STATUS           Status              = EFI_SUCCESS;
    EFI_GUID             BOOT_GUID;
    EFI_GUID             GPP_GUID;
    FW_LINKED_LIST_TYPE *List                 = NULL;
    ACTIVE_GPT_SUB_DATA *BootGPTSubDataPtr    = NULL;
    ACTIVE_GPT_SUB_DATA *GppGPTSubDataPtr     = NULL;
    EFI_GUID             UFS_BOOT_LUN1_GUID   = gEfiUfsLU1Guid;
    EFI_GUID             UFS_BOOT_LUN2_GUID   = gEfiUfsLU2Guid;
    VOID                *CompressedGPTDataPtr = NULL;
    UINT32               CompressedGPTDataSize= 0;
    COMPRESSED_DATA     *CompressedData       = NULL;

    if ((BackupBootGpt == FALSE) &&
        (BackupGppGpt == FALSE))
    {
        return EFI_SUCCESS;
    }

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, CopyPartitionTabletoDPPExit);

    Status = FwGetFwEntryList(&List);
    HANDLE_ERROR_LABEL(Status, FwGetFwEntryList, CopyPartitionTabletoDPPExit);

    if ((CompareGuid(&BOOT_GUID, &UFS_BOOT_LUN1_GUID)) ||
        (CompareGuid(&BOOT_GUID, &UFS_BOOT_LUN2_GUID)))
    {
        DEBUG((EFI_D_WARN,"\n    Skipping backup of GPT table for LUN1/LUN2\n"));
        BackupBootGpt = FALSE;

        Status = AllocateAndInitGPTBackupData(NULL, &GppGPTSubDataPtr);
        HANDLE_ERROR_LABEL(Status, AllocateAndInitGPTBackupData, CopyPartitionTabletoDPPExit);
    }
    else
    {
        Status = AllocateAndInitGPTBackupData(&BootGPTSubDataPtr, &GppGPTSubDataPtr);
        HANDLE_ERROR_LABEL(Status, AllocateAndInitGPTBackupData, CopyPartitionTabletoDPPExit);
    }

    /* Check if it is required to back up GPT of Boot partition */
    if ((BackupBootGpt) && 
        (NULL != BootGPTSubDataPtr))
    {
        Status = GetActiveGPTDataFromGPT (
            BOOT_GUID,
            PRIMARY_GPT_TABLE,
            List,
            BootGPTSubDataPtr);
        HANDLE_ERROR_LABEL(Status, GetActiveGPTDataFromGPT (BOOT_GUID), CopyPartitionTabletoDPPExit);

        CompressedGPTDataSize = BootGPTSubDataPtr->Size;
        /* Giving double size to make sure we dont run short of memory.
           For very small buffers that have really compact data, there
           is a chance that compressed data is larger than the uncompressed
           data. Such case should not fail compression */
        CompressedGPTDataPtr = AllocateZeroPool (CompressedGPTDataSize * 2); 

        Status = CompressGPT(
            BootGPTSubDataPtr,
            BootGPTSubDataPtr->Size,
            CompressedGPTDataPtr,
            &CompressedGPTDataSize);
        HANDLE_ERROR_LABEL(Status, CompressGPT, CopyPartitionTabletoDPPExit);

        Status = AllocateAndInitCompressedData(
            BootGPTSubDataPtr->Size,
            CompressedGPTDataPtr,
            CompressedGPTDataSize,
            &CompressedData);
        HANDLE_ERROR_LABEL(Status, AllocateAndInitCompressedData(BP), CopyPartitionTabletoDPPExit);

        /* Safely backup GPT table to a NV variable */
        Status = SetGPTNvData(QCOM_FW_UPDATE_NV_BP_GPT, CompressedData, CompressedData->Size);
        HANDLE_ERROR_LABEL(Status, SetGPTNvData (QCOM_FW_UPDATE_NV_BP_GPT),CopyPartitionTabletoDPPExit);

        FreePool(CompressedGPTDataPtr);
        CompressedGPTDataPtr = NULL;
    }

    /* Check if it is required to back up GPT of GPP partition */
    if((BackupGppGpt) &&
       (NULL != GppGPTSubDataPtr))
    {
        Status = GetActiveGPTDataFromGPT (
            GPP_GUID,
            PRIMARY_GPT_TABLE,
            List,
            GppGPTSubDataPtr);
        HANDLE_ERROR_LABEL(Status, GetActiveGPTDataFromGPT (GPP_GUID), CopyPartitionTabletoDPPExit);

        CompressedGPTDataSize = GppGPTSubDataPtr->Size;
        /* Giving double size to make sure we dont run short of memory.
           For very small buffers that have really compact data, there
           is a chance that compressed data is larger than the uncompressed
           data. Such case should not fail compression */
        CompressedGPTDataPtr = AllocateZeroPool (CompressedGPTDataSize * 2); 

        Status = CompressGPT(
            GppGPTSubDataPtr,
            GppGPTSubDataPtr->Size,
            CompressedGPTDataPtr,
            &CompressedGPTDataSize);
        HANDLE_ERROR_LABEL(Status, CompressGPT, CopyPartitionTabletoDPPExit);

        Status = AllocateAndInitCompressedData(
            GppGPTSubDataPtr->Size,
            CompressedGPTDataPtr,
            CompressedGPTDataSize,
            &CompressedData);
        HANDLE_ERROR_LABEL(Status, AllocateAndInitCompressedData(GPP), CopyPartitionTabletoDPPExit);

        /* Safely backup GPT table to a NV variable */
        Status = SetGPTNvData(QCOM_FW_UPDATE_NV_GPP_GPT, CompressedData, CompressedData->Size);
        HANDLE_ERROR_LABEL(Status, SetGPTNvData (QCOM_FW_UPDATE_NV_GPP_GPT),CopyPartitionTabletoDPPExit);

        FreePool(CompressedGPTDataPtr);
        CompressedGPTDataPtr = NULL;
    }

CopyPartitionTabletoDPPExit:

    if (BootGPTSubDataPtr != NULL)
    {
        FreePool(BootGPTSubDataPtr);
        BootGPTSubDataPtr = NULL;
    }

    if (GppGPTSubDataPtr != NULL)
    {
        FreePool(GppGPTSubDataPtr);
        GppGPTSubDataPtr = NULL;
    }

    if (NULL != CompressedGPTDataPtr)
    {
        FreePool(CompressedGPTDataPtr);
        CompressedGPTDataPtr = NULL;
    }
    return Status;
}

/**
 * Compare GPT with the table in FAT partition and check if it the swapped table 
 * or it is the primary original table 
 *
 * @param TableType       :   Type of GPT Table 
 * @param DiskType_GUID   :   Vendor GUID whose GPT has to be 
 *                            compared
 * @param gptTableState   :   returns if GPT table is in the
 *                            original or switched state
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS 
CmpGPTTable(
    IN  GPT_TABLE_TYPE   TableType,
    IN  EFI_GUID         DiskType_GUID,
    OUT GPT_TABLE_STATE *gptTableState
    )
{
    EFI_STATUS             Status                = EFI_SUCCESS;
    VOID                  *GPTfromNV             = NULL;
    VOID                  *GPTData               = NULL;
    UINTN                  GPTfromNVSize        = 0;
    UINTN                  GPTDataSize           = 0;
    EFI_PARTITION_ENTRY   *PartitionEntryfromGPT = NULL;
    EFI_PARTITION_ENTRY   *PartitionEntryfromFAT = NULL;
    EFI_GUID               Emmc_BOOT_GUID        = gEfiEmmcBootPartition1Guid;
    EFI_GUID               Emmc_GPP_GUID         = gEfiEmmcGppPartition1Guid;
    EFI_GUID               UFS_BOOT1_GUID        = gEfiUfsLU1Guid;
    EFI_GUID               UFS_BOOT2_GUID        = gEfiUfsLU2Guid;
    EFI_GUID               UFS_LUN4_GUID         = gEfiUfsLU4Guid;
    EFI_GUID               SPINOR_GUID           = gEfiSpiNor0Guid;
    EFI_GUID               NVME_GUID             = gEfiNvme0Guid;
    QCOM_FW_UPDATE_NV_TYPE GPTNvType             = QCOM_FW_UPDATE_NV_MAX;
    COMPRESSED_DATA       *CompressedDataPtr     = NULL;
    UINTN                  CompressedDataSize    = 0;
    UINT32                 GPTSubDataSize        = 0;
    ACTIVE_GPT_SUB_DATA   *GPTSubDataPtr         = NULL;

    // Check input parameters
    if(CompareGuid(&DiskType_GUID, &Emmc_BOOT_GUID) ||
       CompareGuid(&DiskType_GUID, &UFS_BOOT1_GUID) ||
       CompareGuid(&DiskType_GUID, &UFS_BOOT2_GUID) ||
       CompareGuid(&DiskType_GUID, &SPINOR_GUID))
    {
        GPTNvType = QCOM_FW_UPDATE_NV_BP_GPT;
    }
    else if(CompareGuid(&DiskType_GUID, &Emmc_GPP_GUID) ||
            CompareGuid(&DiskType_GUID, &UFS_LUN4_GUID) ||
            CompareGuid(&DiskType_GUID, &NVME_GUID))
    {
        GPTNvType = QCOM_FW_UPDATE_NV_GPP_GPT;
    }
    else
    {
        Status = EFI_INVALID_PARAMETER;
        return Status;
    }
    
    Status = ReadGPTTable(DiskType_GUID, &GPTData, &GPTDataSize, NULL, TableType);
    HANDLE_ERROR_LABEL(Status,ReadGPTTable,CmpGPTTableExit);

    Status = GetGPTNvData(GPTNvType, (VOID **)&CompressedDataPtr, &CompressedDataSize);       
    HANDLE_ERROR_LABEL(Status,GetGPTNvData,CmpGPTTableExit);
    
    GPTSubDataSize = CompressedDataPtr->UncompressedSize;
    GPTSubDataPtr = AllocateZeroPool (GPTSubDataSize);
    Status = (NULL == GPTSubDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
    HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GPTSubDataPtr),  CmpGPTTableExit);
    
    Status = DecompressGPT(
        CompressedDataPtr->Data,
        (CompressedDataPtr->Size - OFFSET_OF(COMPRESSED_DATA, Data)),
        GPTSubDataPtr,
        &GPTSubDataSize);
    HANDLE_ERROR_LABEL(Status,DecompressGPT,CmpGPTTableExit);

    // Derive the original GPT table using GPT sub data
    Status = GetGPTFromActiveGPTData (
        DiskType_GUID,
        TableType,
        GPTSubDataPtr,
        &GPTfromNV,
        &GPTfromNVSize,
        NULL
        );
    HANDLE_ERROR_LABEL(Status,GetGPTFromActiveGPTData,CmpGPTTableExit);

    PartitionEntryfromGPT = (EFI_PARTITION_ENTRY *)GPTData;
    PartitionEntryfromFAT = (EFI_PARTITION_ENTRY *)GPTfromNV;

    if(CompareMem((VOID*)PartitionEntryfromGPT, (VOID*)PartitionEntryfromFAT, GPTDataSize) == 0) 
    {
        *gptTableState= ORIGINAL_TABLE;  
    }
    else 
    {
        *gptTableState= SWITCHED_TABLE;
    }
    Status = EFI_SUCCESS;

CmpGPTTableExit:

    if(GPTData != NULL)
    {
        gBS->FreePool(GPTData);
        GPTData = NULL;
    }

    if(GPTfromNV != NULL)
    {
        gBS->FreePool(GPTfromNV);
        GPTfromNV = NULL;
    }

    if (NULL != GPTSubDataPtr)
    {
        FreePool (GPTSubDataPtr);
        GPTSubDataPtr = NULL;
    }

    if (NULL != CompressedDataPtr)
    {
        FreePool (CompressedDataPtr);
        CompressedDataPtr = NULL;
    }

    return Status;
}
       

/**
 * This function goes through the firmware entry list and checks
 * if backup GPT of BOOT and GPP/LUN4 partitions needs to be 
 * switched for fail-safe update. 
 *  
 * @param List          :  Firmware entry list 
 * @param SwitchBootGpt :  Upon return, indicates if BOOT backup
 *                      GPT needs to be switched
 * @param SwitchGppGpt  :  Upon return, indicates if GPP/LUN4 backup 
 *                      GPT needs to be switched
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS 
GetAllSwitchGptRequestsFromFwEntryList(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *SwitchBootGpt,
    OUT BOOLEAN            *SwitchGppGpt
    )
{
    EFI_STATUS Status         = EFI_SUCCESS;
    EFI_GUID   BOOT_GUID;
    EFI_GUID   GPP_GUID;

    if ((List == NULL) ||
        (SwitchBootGpt == NULL) ||
        (SwitchGppGpt == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
    HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, GetAllSwitchGptRequestsFromFwEntryListExit);
  
    Status = SwitchBackupGptTableEntries(BOOT_GUID, List, FALSE, SwitchBootGpt);
    HANDLE_ERROR_LABEL(Status,SwitchBackupGptTableEntries,GetAllSwitchGptRequestsFromFwEntryListExit);

    Status = SwitchBackupGptTableEntries(GPP_GUID, List, FALSE, SwitchGppGpt);
    HANDLE_ERROR_LABEL(Status,SwitchBackupGptTableEntries,GetAllSwitchGptRequestsFromFwEntryListExit);

GetAllSwitchGptRequestsFromFwEntryListExit:

    return Status;

}

/**
 * Function to allocate memory for GPT sub data with for the given length
 *
 * Use FreePool() to free the memory allocated by this function
 *
 * @param MaxEntries    :  Max entries allowed in the GPT array
 * @param GPTSubData    :  GPT sub data allocated and initialized
 *                         GPT needs to be switched
 * 
 * @return EFI_STATUS
 */
EFI_STATUS
AllocateAndInitGPTSubData(
    IN UINT32                  MaxEntries,
    OUT ACTIVE_GPT_SUB_DATA  **GPTSubData
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                GPTSubDataSize = 0;

  if ((NULL == GPTSubData) ||
      (0 == MaxEntries))
  {
    return EFI_INVALID_PARAMETER;
  }

  GPTSubDataSize = 
      sizeof (ACTIVE_GPT_SUB_DATA) + 
      (sizeof (ACTIVE_GPT_ENTRY_SUB_DATA) * (MaxEntries - 1));
  
  *GPTSubData = AllocateZeroPool (GPTSubDataSize);
  
  Status = (NULL == *GPTSubData) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
  HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GPTSubDataSize),  AllocateAndInitGPTSubDataExit);

  (*GPTSubData)->MaxEntries = MaxEntries;
  (*GPTSubData)->Size = GPTSubDataSize;

  return EFI_SUCCESS;

 AllocateAndInitGPTSubDataExit:

  if (*GPTSubData != NULL)
  {
    FreePool(*GPTSubData);
    *GPTSubData = NULL;
  }

  return Status;
}

/**
 * Function to allocate memory for GPT sub data with for the given length
 *
 * @param UncompressedSize:  Data size before compression happens
 * @param InputData       :  Optional parameter. If supplied, needs
 *                           to be copied to the COMPRESSED_DATA->Data
 * @param InputDataSize   :  Compressed data size
 * @param CompressedData  :  To hold compressed data
 * 
 * @return EFI_STATUS
 */
EFI_STATUS
AllocateAndInitCompressedData(
    IN UINT32                  UncompressedSize,
    IN OPTIONAL VOID          *InputData,
    IN UINT32                  InputDataSize,
    OUT COMPRESSED_DATA      **CompressedData
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                CompressedDataSize = 0;

  if ((NULL == CompressedData) ||
      (0 == UncompressedSize))
  {
    return EFI_INVALID_PARAMETER;
  }

  CompressedDataSize = 
      OFFSET_OF(COMPRESSED_DATA, Data) +
      InputDataSize;

  *CompressedData = AllocateZeroPool (CompressedDataSize);
  
  Status = (NULL == *CompressedData) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
  HANDLE_ERROR_LABEL(Status, AllocateZeroPool (CompressedDataSize),  AllocateAndInitCompressedDataExit);

  (*CompressedData)->Size = CompressedDataSize;
  (*CompressedData)->UncompressedSize = UncompressedSize;

  if (NULL != InputData)
  {
    CopyMem(&(*CompressedData)->Data, InputData, InputDataSize);
  }

  return EFI_SUCCESS;

 AllocateAndInitCompressedDataExit:

  if (*CompressedData != NULL)
  {
    FreePool(*CompressedData);
    *CompressedData = NULL;
  }

  return Status;
}

void *myalloc(
    void *Opaque, 
    unsigned Items, 
    unsigned Size
)
{
    Opaque = Z_NULL;
    return AllocatePool (Items * Size);
}

void myfree(
    void *Opaque,
    void *Addr
)
{
    Opaque = Z_NULL;
    FreePool(Addr);
}

/**
 * This function compresses the SourceBuffer of SourceSize and the
 * output will be copied to DestinationBuffer, respective DestinationSize 
 * will be updated with compressed size.
 *  
 * @param SourceBuffer          Buffer to compress 
 * @param SourceSize     :      Size of source buffer
 * @param DestinationBuffer  :  Destination buffer to store compressed data
 * @param DestinationSize  :    Destination buffer size
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
CompressGPT (
  IN VOID       *SourceBuffer,
  IN UINT32      SourceSize,
  IN OUT VOID   *DestinationBuffer,
  IN OUT UINT32 *DestinationSize
  )
{
  z_stream  Stream;
  INT32     Ret;

  if (SourceBuffer == NULL || 
      !SourceSize ||
      DestinationBuffer == NULL ||
      DestinationSize == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Stream.zalloc = myalloc;
  Stream.zfree = myfree;
  Stream.opaque = Z_NULL;

  Stream.next_in  = SourceBuffer; 
  Stream.avail_in = SourceSize;
  
  Stream.next_out  = DestinationBuffer; 
  Stream.avail_out = *DestinationSize;

  Ret = deflateInit2(
    &Stream,
    Z_BEST_COMPRESSION,
    Z_DEFLATED,
    MAX_WBITS,
    MAX_MEM_LEVEL,
    Z_DEFAULT_STRATEGY
    );
  if (Ret != Z_OK)
  {
    DEBUG ((EFI_D_WARN, "deflateInit2 failed!. Ret - %d\n", Ret));
    return EFI_OUT_OF_RESOURCES;
  }

  Ret = deflate(
    &Stream, 
    Z_FINISH
    );
  if (Ret != Z_STREAM_END) {
      DEBUG ((EFI_D_WARN, "deflate failed!. Ret - %d\n", Ret));
      deflateEnd(&Stream);
      return  Ret == Z_OK ? EFI_BUFFER_TOO_SMALL : EFI_NOT_COMPRESSED;
  }

  *DestinationSize = Stream.total_out;

  (VOID) deflateEnd(&Stream);

#ifdef DEBUG
  DEBUG ((EFI_D_ERROR, "    GPT-Data Compression Statistics:: OriginalSize: %d, CompressedSize: %d\n\n", SourceSize, *DestinationSize));
#endif // #ifdef DEBUG

  return EFI_SUCCESS;
}

/**
 * This function decompresses the SourceBuffer of SourceSize and the
 * output will be copied to DestinationBuffer, respective DestinationSize 
 * will be updated with decompressed size.
 *  
 * @param SourceBuffer          Buffer to Decompress 
 * @param SourceSize     :      Size of source buffer
 * @param DestinationBuffer  :  Destination buffer to store decompressed data
 * @param DestinationSize  :    Destination buffer size
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
DecompressGPT (
  IN VOID       *SourceBuffer,
  IN UINT32      SourceSize,
  IN OUT VOID   *DestinationBuffer,
  IN OUT UINT32 *DestinationSize
  )
{
  z_stream  Stream;
  INT32     Ret;

  if (SourceBuffer == NULL || 
      !SourceSize ||
      DestinationBuffer == NULL ||
      !DestinationSize)
  {
    return EFI_INVALID_PARAMETER;
  }

  Stream.zalloc = myalloc;
  Stream.zfree = myfree;
  Stream.opaque = Z_NULL;

  Stream.next_in  = SourceBuffer; 
  Stream.avail_in = SourceSize;
  
  Stream.next_out  = DestinationBuffer; 
  Stream.avail_out = *DestinationSize;

  Ret = inflateInit2(
    &Stream,
    MAX_WBITS
    );
  if (Ret != Z_OK)
  {
    DEBUG ((EFI_D_WARN, "inflateInit2 failed!. Ret - %d\n", Ret));
    return EFI_OUT_OF_RESOURCES;
  }

  Ret = inflate(
    &Stream, 
    Z_FINISH
    );
  if (Ret != Z_STREAM_END) {
      DEBUG ((EFI_D_WARN, "inflate failed!. Ret - %d\n", Ret));
      inflateEnd(&Stream);
      if (Ret == Z_NEED_DICT || (Ret == Z_BUF_ERROR && Stream.avail_in == 0)) {
          return EFI_COMPROMISED_DATA;
      }
      return EFI_ABORTED;
  }

  *DestinationSize = Stream.total_out;

  (VOID) inflateEnd(&Stream);

  return EFI_SUCCESS;
}

