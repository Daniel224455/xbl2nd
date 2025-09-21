/** @file FwPayloadEntry.h

  Payload entry structures and functions firmware update solution.

  Copyright (c) 2012-2013,2015-2018 Qualcomm Technologies, Inc.  All Rights
  Reserved. Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 ----------     ---         --------------------------------------------------
 2018/03/23     prema       Handled capsule result status to Unsupported when payload 
                            used with all IGNORE and empty Zero payloads capsule.
 2017/06/21     gparam      Change to return payload files parsed from the payload FV
                            in a newly allocated buffer instead of pointing to location
                            within the FV
 2017/05/30     gparam      Change to clean global link list of firmware entry list
                            every time before starting a fresh parsing
 2017/05/08     gparam      Changes to do same processing in FwEntry combination
                            checking for FWCLASS_GUID as done in all other
                            existing DPP provisioning FWEntry types
 2017/04/22     gparam      Changes to generalize FindMetaData function and remove
                            the hard dependency on global variable PayLoadHeader +
                            Changed the update status from internal type to
                            standard ones defined in EDK headers 
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers
 2017/04/18     gparam      Fixed the flash type validation condition check to not
                            return error for update types FWENTRY_DISK_PARTITION_DONT_CARE +
                            Update to apply the logic that checks and updates the
                            XBL FwEntry only for firmware entries that has
                            UpdateType equal to FWENTRY_UPDATE_TYPE_PARTITION
 2017/04/18     gparam      Corrected the UpdateType string array contents to be
                            in-par with corresponding UpdateType enum names
 2017/02/22     ssumit      Added payload buffer and size to support SetImage function
 2017/02/22     pkn         Added changes required to support new update type - FWCLASS_GUID
 2017/02/13     gpavithr    Set UPDATE disk partition type & BACKUP disk partition
                            type for XBL based on CurrentBootLUN
 2016/12/07     ssumit      Changes to replace #define GUIDs with extern variables
 2016/08/29     ltg         Added UFS support
 2015/04/15     mic         Added 64bit firmware update support
 2015/04/22     mic         Removed SSD decryption support
 2013/05/06     mic         Unsupport V1/V2 payload
 2013/02/15     rs          Updated to always try to decrypt FwEntry
 2012/12/11     mic         Fixed KW warnings
 2012/11/16     mic         Added payload header print function
 2012/11/15     mic         Added Fail-safe fix on gpt switched case
 2012/09/21     mic         Initial version

=============================================================================*/
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/FwCommonLib.h>
#include <Library/FwUpdateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/BootConfig.h>
#include <Library/EdkiiSystemCapsuleLib.h>

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/BlockIo.h>
#include "FwPayloadEntry.h"
#include "FwCapsule.h"
#include "FwEntryIO_helpers.h"

const CHAR16 *sOperationType[FWENTRY_OPERATION_TYPE_MAX] = {L"IGNORE",
                                                            L"UPDATE",
                                                            L"DELETE"
                                                           };

const CHAR16 *sUpdateType[FWENTRY_UPDATE_TYPE_MAX]       = {L"Partition",
                                                            L"FAT_FILE",
                                                            L"DPP_QCOM",
                                                            L"DPP_OEM",
                                                            L"OPM_PRIV_KEY",
                                                            L"SSD",
                                                            L"FWCLASS_GUID"
                                                           };

const CHAR16 *sBackUpType[FWENTRY_BACKUP_TYPE_MAX]       = {L"Partition",
                                                            L"FAT"
                                                           };

const CHAR16 *sDiskType[FWENTRY_DISK_PARTITION_MAX]      = {L"USER DATA",
                                                            L"BOOT1",
                                                            L"BOOT2",
                                                            L"RPMB",
                                                            L"GPP1",
                                                            L"GPP2",
                                                            L"GPP3",
                                                            L"GPP4",
                                                            L"LUN0",
                                                            L"LUN1",
                                                            L"LUN2",
                                                            L"LUN3",
                                                            L"LUN4",
                                                            L"LUN5",
                                                            L"LUN6",
                                                            L"LUN7"
                                                           };

extern const EFI_GUID *FwEntryDiskPartitionGuids[];

/*FwEntry LinkedList */
FW_LINKED_LIST_TYPE         FwEntryList         = {0, NULL,FwEntryDestructor};
FV_BUFFER_HANDLE            FVBufferHandle      = {0};
QPAYLOAD_METADATA_HEADER_V3 PayLoadHeader       = {0};
BOOLEAN                     bDoubleUpdateNeeded = FALSE;

static EFI_GUID gEfiDppFatPartitionGuid =
  {0xebd0a0a2, 0xb9e5, 0x4433, {0x87, 0xc0, 0x68, 0xb6, 0xb7, 0x26, 0x99, 0xc7}};

/**
 * Function to get FV buffer handle for the given buffer and buffer-size
 *
 * @param FvHandle - FV buffer handle

 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetFvBufferHandle(
  IN VOID                 *Image,
  IN UINTN                 ImageSize,
  OUT FV_BUFFER_HANDLE    *FvBufferHandle
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (FvBufferHandle == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  FvBufferHandle->Buffer = Image;
  FvBufferHandle->BufferSize = ImageSize;

  return Status;
}

/**
 * Function to print Payload header
 *
 * @param payloadHeader - Payload Header
 * @param DebugLevel    - Debug Level
 * @return : EFI_STATUS
 */
EFI_STATUS
FwPrintPayloadHeader(
  IN QPAYLOAD_METADATA_HEADER_V3  *payloadHeader,
  IN UINTN                         DebugLevel
  )
{

    DEBUG((DebugLevel,"    Payload info:\n"));
    DEBUG((DebugLevel,"    ====================================\n"));

    DEBUG((DebugLevel,"    Revision                 : %d\n", payloadHeader->Revision));
    DEBUG((DebugLevel,"    Header size (Byte)       : %d\n", payloadHeader->Size));
    DEBUG((DebugLevel,"    FwVer                    : 0x%x\n", payloadHeader->FirmwareVersion));
    DEBUG((DebugLevel,"    Lowest supported version : 0x%x\n", payloadHeader->LowestSupportedVersion));
    if (payloadHeader->Revision > 0x1)
    {
        DEBUG((DebugLevel,"    Breaking change number   : %d\n", payloadHeader->BreakingChangeNumber));
    }
    if (payloadHeader->Revision > 0x2)
    {
        DEBUG((DebugLevel,"    Entry count              : %d\n", payloadHeader->EntryCount));
    }

    DEBUG((DebugLevel,"    ====================================\n"));

    return EFI_SUCCESS;
}

/**
 * Function to print FwEntry
 *
 * @param pFwEntry - pointer point to FwEntry
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwPrintFwEntry(
    IN QPAYLOAD_FW_ENTRY *pFwEntry,
    IN UINTN             DebugLevel
    )
{
    UINT32       CurrentBootLun    = 0xFFFFFFFF;
    EFI_STATUS   Status            = EFI_SUCCESS;
    UINT32       DiskPartitionType = FWENTRY_DISK_PARTITION_MAX;

    if(pFwEntry == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetBootLUN(&CurrentBootLun);
    HANDLE_ERR_CONTINUE(Status, GetBootLUN);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN,"Using the XBL DiskPartitionType value for Update & Backup from the FwEntry in metadata\n"));
    }

    DEBUG((DebugLevel,"    FwEntry:\n"));
    DEBUG((DebugLevel,"    ====================================\n"));

    if(DebugLevel == EFI_D_ERROR)
    {
        /*FileGuid*/
        DEBUG((DebugLevel,"      FileGuid             = "));
        FwCommonPrintGuid(pFwEntry->Metadata.FileGuid,DebugLevel);
        DEBUG((DebugLevel,"\n"));
    }

    /*Operation Type*/
    DEBUG((DebugLevel,"      Operation            = 0x%x,", pFwEntry->Metadata.Operation));
    if(pFwEntry->Metadata.Operation < FWENTRY_OPERATION_TYPE_MAX)
    {
        DEBUG((DebugLevel,"{%s}\n",sOperationType[pFwEntry->Metadata.Operation]));
    }
    else
    {
        DEBUG((DebugLevel,"{Unknown}\n"));
    }
    DEBUG((DebugLevel,"\n"));

    /*Update Type and Path*/
    DEBUG((DebugLevel,"      UpdateType           = 0x%x,", pFwEntry->Metadata.UpdateType));
    if(pFwEntry->Metadata.UpdateType< FWENTRY_UPDATE_TYPE_MAX)
    {
        DEBUG((DebugLevel,"{%s}\n",sUpdateType[pFwEntry->Metadata.UpdateType]));
    }
    else
    {
        DEBUG((DebugLevel,"{Unknown}\n"));
    }

    if(pFwEntry->Metadata.UpdatePath.DiskPartitionType < FWENTRY_DISK_PARTITION_MAX)
    {
        if(StrnCmp(pFwEntry->Metadata.UpdatePath.PartitionName, XBL_PARTITION_NAME, StrLen(XBL_PARTITION_NAME)) == 0)
        {
            if(CurrentBootLun == UFS_LUN_1)
            {
                DiskPartitionType = FWENTRY_UFS_LUN2;
            }
            else if(CurrentBootLun == UFS_LUN_2)
            {
                DiskPartitionType = FWENTRY_UFS_LUN1;
            }
            else
            {
                DiskPartitionType = pFwEntry->Metadata.UpdatePath.DiskPartitionType;
            }

            DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,", DiskPartitionType));
            DEBUG((DebugLevel,"{%s}\n", sDiskType[DiskPartitionType]));
        }
        else
        {
            DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,", pFwEntry->Metadata.UpdatePath.DiskPartitionType));
            DEBUG((DebugLevel,"{%s}\n", sDiskType[pFwEntry->Metadata.UpdatePath.DiskPartitionType]));
        }
    }
    else
    {
        DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,{Unknown}\n", pFwEntry->Metadata.UpdatePath.DiskPartitionType));
    }

    DEBUG((DebugLevel,"        PartitionName      = %s\n", pFwEntry->Metadata.UpdatePath.PartitionName));

    if(DebugLevel == EFI_D_ERROR)
    {
        DEBUG((DebugLevel,"        PartitionTypeGUID  = "));
        FwCommonPrintGuid(pFwEntry->Metadata.UpdatePath.PartitionTypeGUID,DebugLevel);
        DEBUG((DebugLevel,"\n"));
    }

    DEBUG((DebugLevel,"        FileName           = %s\n", pFwEntry->Metadata.UpdatePath.FileName));
    DEBUG((DebugLevel,"\n"));

    /*Backup Type and Path*/
    DEBUG((DebugLevel,"      BackupType           = 0x%x,", pFwEntry->Metadata.BackupType));
    if(pFwEntry->Metadata.BackupType< FWENTRY_BACKUP_TYPE_MAX)
    {
        DEBUG((DebugLevel,"{%s}\n",sBackUpType[pFwEntry->Metadata.BackupType]));
    }
    else
    {
        DEBUG((DebugLevel,"{Unknown}\n"));
    }

    if(pFwEntry->Metadata.BackupPath.DiskPartitionType < FWENTRY_DISK_PARTITION_MAX)
    {
        if(StrnCmp(pFwEntry->Metadata.BackupPath.PartitionName, XBL_PARTITION_NAME, StrLen(XBL_PARTITION_NAME)) == 0)
        {
            if(CurrentBootLun == UFS_LUN_1)
            {
                DiskPartitionType = FWENTRY_UFS_LUN1;
            }
            else if(CurrentBootLun == UFS_LUN_2)
            {
                DiskPartitionType = FWENTRY_UFS_LUN2;
            }
            else
            {
                DiskPartitionType =  pFwEntry->Metadata.BackupPath.DiskPartitionType;
            }

            DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,", DiskPartitionType));
            DEBUG((DebugLevel,"{%s}\n", sDiskType[DiskPartitionType]));
        }
        else
        {
            DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,", pFwEntry->Metadata.BackupPath.DiskPartitionType));
            DEBUG((DebugLevel,"{%s}\n", sDiskType[pFwEntry->Metadata.BackupPath.DiskPartitionType]));
        }
    }
    else
    {
        DEBUG((DebugLevel,"        DiskPartitionType  = 0x%x,{Unknown}\n", pFwEntry->Metadata.BackupPath.DiskPartitionType));
    }

    DEBUG((DebugLevel,"        PartitionName      = %s\n", pFwEntry->Metadata.BackupPath.PartitionName));

    if(DebugLevel == EFI_D_ERROR)
    {
        DEBUG((DebugLevel,"        PartitionTypeGUID  = "));
        FwCommonPrintGuid(pFwEntry->Metadata.BackupPath.PartitionTypeGUID,DebugLevel);
        DEBUG((DebugLevel,"\n"));
    }

    DEBUG((DebugLevel,"        FileName           = %s\n", pFwEntry->Metadata.BackupPath.FileName));
    DEBUG((DebugLevel,"\n"));
  #if 0
    /*Crypto info*/
    DEBUG((DebugLevel,"      Encrypted            = 0x%x,", pFwEntry->IsEncrypted));
    if(pFwEntry->IsEncrypted)
    {
        DEBUG((DebugLevel,"True\n"));
    }
    else
    {
        DEBUG((DebugLevel,"False\n"));
    }
  #endif
    if (DebugLevel == EFI_D_ERROR)
    {
        DEBUG((DebugLevel,"        Decrypted address  = 0x%x\n", pFwEntry->DecryptedFwEntryBuffer));
        DEBUG((DebugLevel,"        Decrypted size     = 0x%x\n", pFwEntry->DecryptedFwEntrySize));
        DEBUG((DebugLevel,"\n      Handle address\n"));

        /*Handle info*/
        DEBUG((DebugLevel,"        Source             = 0x%x\n", pFwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_SOURCE]));
        DEBUG((DebugLevel,"        Dest               = 0x%x\n", pFwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST]));
        DEBUG((DebugLevel,"        Backup             = 0x%x\n", pFwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP]));

    }

    DEBUG((DebugLevel,"    ====================================\n"));

    return EFI_SUCCESS;
};

/**
 * Function to print FwEntry list
 *
 * @param List - FwEntry LinkedList
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwPrintFwList(
    IN FW_LINKED_LIST_TYPE *List,
    IN UINTN               DebugLevel
    )
{
    EFI_STATUS          Status        = EFI_SUCCESS;
    UINTN               NodeCount     = 0;
    FW_LIST_NODE_TYPE   *Node         = NULL;
    UINTN               i             = 0;

    if (List == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    //Get a pointer to the head node
    Status = LinkedListGetHeadNode(List, &Node);
    HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwPrintFwListExit);

    if (Node == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    //Get number of nodes
    Status= LinkedListGetNodeCount(List, &NodeCount);
    HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwPrintFwListExit);

    for (i = 0; i < NodeCount; i++)
    {
        Status = FwPrintFwEntry((QPAYLOAD_FW_ENTRY*) Node->Data,DebugLevel);
        HANDLE_ERROR_LABEL(Status,FwPrintFwEntry,FwPrintFwListExit);
        Node = Node->Next;
    }

FwPrintFwListExit:

    return EFI_SUCCESS;
};

/**
 * Function to destruct   QPAYLOAD_FW_ENTRY
 *
 * @param Node - node for destruction
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryDestructor(
    IN VOID *Data
    )
{
    QPAYLOAD_FW_ENTRY *pFwEntry = NULL;

    if( Data == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    pFwEntry = (QPAYLOAD_FW_ENTRY*)Data;

    if(pFwEntry->DecryptedFwEntryBuffer != NULL)
    {
        /*The decrypted buffer is always uncached*/
        gBS->FreePool(pFwEntry->DecryptedFwEntryBuffer);
        pFwEntry->DecryptedFwEntryBuffer = NULL;
    }

    gBS->FreePool(pFwEntry);
    pFwEntry = NULL;

    return EFI_SUCCESS;
}

/**
 * Function to get  FwEntry linkedlist handle
 *
 * @param List - double pointer of FwEntry list

 * @return : EFI_STATUS
 */
EFI_STATUS
FwGetFwEntryList(
    OUT FW_LINKED_LIST_TYPE **List
    )
{
    if(List == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    if((FwEntryList.Count == 0) || (FwEntryList.Head == NULL))
    {
        return EFI_NOT_FOUND;
    }

    *List = &FwEntryList;
    return EFI_SUCCESS;
};

/**
 * Function to get the Metadata
 *
 * @param Image             - Payload to parse
 * @param ImageSize         - Payload size
 * @param Version           - metadata version
 * @param PayloadHeader     - Payload header structure (optional)
 * @param PayloadHeaderSize - Payload header structure size (optional but,
 *                            mandatory if PayloadHeader is non-NULL)
 * @param FileBuffer        - Metadata buffer (optional)
 * @param BufferSize        - Metadata size (optional but,
 *                            mandatory if FileBuffer is non-NULL)
 * @param MsftAttemptStatus - MSFT defined error status
 * @param QcResultFlag      - QCOM defined error status
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwFindMetadata(
    IN VOID                      *Image,
    IN UINTN                      ImageSize,
    OUT UINT32                   *Version,
    OUT VOID                     *PayloadHeader,      OPTIONAL
    OUT UINTN                    *PayloadHeaderSize,  OPTIONAL
    OUT VOID                    **FileBuffer,         OPTIONAL
    OUT UINTN                    *BufferSize,         OPTIONAL
    OUT UINT32                   *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{

    EFI_STATUS                    Status     = EFI_SUCCESS;
    QPAYLOAD_METADATA_HEADER_V1  *pMetaData  = NULL;
    EFI_GUID                      mGuid      = QCOM_FW_UPDATE_FILE_VERSION_METADATA_IN_PAYLOAD_GUID;
    UINT32                        FileHeaderSize = 0;
    VOID                         *ffsFileBuffer = NULL;
    UINTN                         ffsBufferSize = 0;
    BOOLEAN                       ReadResult = FALSE;

    if((Version == NULL) ||
       (Image == NULL) ||
       (MsftAttemptStatus == NULL) ||
       (QcResultFlag == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    if (((FileBuffer != NULL) && (BufferSize == NULL)) ||
        ((FileBuffer == NULL) && (BufferSize != NULL)))
    {
        return EFI_INVALID_PARAMETER;
    }
      
    if (((PayloadHeader != NULL) && (PayloadHeaderSize == NULL)) ||
        ((PayloadHeader == NULL) && (PayloadHeaderSize != NULL)))
    {
        return EFI_INVALID_PARAMETER;
    }

    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_ERR_METADATA_INVALID_PARAM;

    Status = FwUpdateGetFvBufferHandle(Image,ImageSize,&FVBufferHandle);
    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_METADATA_GET_FV_HANDLE;
        HANDLE_ERROR_LABEL(Status,FwUpdateGetFvBufferHandle,FwFindMetadataExit);
    }

    ReadResult = GetFfsByName(
        FVBufferHandle.Buffer,
        FVBufferHandle.BufferSize,
        &mGuid,
        EFI_FV_FILETYPE_RAW,
        &ffsFileBuffer,
        &ffsBufferSize
        );

    if (FALSE == ReadResult)
    {
        Status = EFI_NOT_FOUND;
        *QcResultFlag = QCFW_UPDATE_ERR_METADATA_FV_READ;
        HANDLE_ERROR_LABEL(Status,GetFfsByName,FwFindMetadataExit);
    }

    if (IS_FFS_FILE2 (ffsFileBuffer)) {
        FileHeaderSize = sizeof(EFI_FFS_FILE_HEADER2);
    } else {
        FileHeaderSize = sizeof(EFI_FFS_FILE_HEADER);
    }
    ffsFileBuffer = (UINT8 *)ffsFileBuffer + FileHeaderSize;
    ffsBufferSize = ffsBufferSize - FileHeaderSize;

    /*Buffer size checking*/
    if(ffsBufferSize >= sizeof(QPAYLOAD_METADATA_HEADER_V1))
    {
        pMetaData = (QPAYLOAD_METADATA_HEADER_V1*)ffsFileBuffer;

        if((pMetaData->Signature1 == QPAYLOAD_METADATA_SIGNATURE1) &&
            (pMetaData->Signature2 == QPAYLOAD_METADATA_SIGNATURE2) )
        {
            *Version = pMetaData->Revision;
        }
        else
        {
            Status = EFI_NOT_FOUND;
            *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
            *QcResultFlag = QCFW_UPDATE_ERR_METADATA_BAD_SIGNATURE;
            DEBUG_ERR_LABEL("    Bad metadata signature\n",FwFindMetadataExit);
        }
    }
    else
    {
        Status = EFI_BAD_BUFFER_SIZE;
        *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
        *QcResultFlag = QCFW_UPDATE_ERR_METADATA_BAD_HEADER_SIZE;
        DEBUG_ERR_LABEL("    Bad metadata header size\n",FwFindMetadataExit);
    }

    /*Get Payload metadata header*/
    switch(*Version)
    {
        case 0:
        case 1:
            /* Validate the payload buffer size with the current payload version */
            if ((PayloadHeaderSize != NULL) &&
                (*PayloadHeaderSize < sizeof(QPAYLOAD_METADATA_HEADER_V1)))
            {
                Status = EFI_BUFFER_TOO_SMALL;
                *PayloadHeaderSize = sizeof(QPAYLOAD_METADATA_HEADER_V1);
                goto FwFindMetadataExit;
            }

            if(ffsBufferSize == sizeof(QPAYLOAD_METADATA_HEADER_V1))
            {
                DEBUG((EFI_D_WARN,"    V1 payload found\n"));
                if (NULL != PayloadHeader)
                {
                    CopyMem((VOID*)PayloadHeader,ffsFileBuffer,sizeof(QPAYLOAD_METADATA_HEADER_V1));
                }
            }
            else
            {
                Status = EFI_BAD_BUFFER_SIZE;
                *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
                *QcResultFlag = QCFW_UPDATE_ERR_METADATA_BAD_V1_HEADER_SIZE;
                DEBUG_ERR_LABEL("    Bad metadata V1 header size\n",FwFindMetadataExit);
            }
            break;

        case 2:
            /* Validate the payload buffer size with the current payload version */
            if ((PayloadHeaderSize != NULL) &&
                (*PayloadHeaderSize < sizeof(QPAYLOAD_METADATA_HEADER_V2)))
            {
                Status = EFI_BUFFER_TOO_SMALL;
                *PayloadHeaderSize = sizeof(QPAYLOAD_METADATA_HEADER_V2);
                goto FwFindMetadataExit;
            }

            if(ffsBufferSize == sizeof(QPAYLOAD_METADATA_HEADER_V2))
            {
                DEBUG((EFI_D_WARN,"    V2 payload found\n"));
                if (NULL != PayloadHeader)
                {
                    CopyMem((VOID*)PayloadHeader,ffsFileBuffer,sizeof(QPAYLOAD_METADATA_HEADER_V2));
                }
            }
            else
            {
                Status = EFI_BAD_BUFFER_SIZE;
                *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
                *QcResultFlag = QCFW_UPDATE_ERR_METADATA_BAD_V2_HEADER_SIZE;
                DEBUG_ERR_LABEL("    Bad metadata V2 header size\n",FwFindMetadataExit);
            }
            break;

        /*Version >= 3 should use V3 header, this provides extendable payload version
          support*/
        case 3:
        default:
            /* Validate the payload buffer size with the current payload version */
            if ((PayloadHeaderSize != NULL) &&
                (*PayloadHeaderSize < sizeof(QPAYLOAD_METADATA_HEADER_V3)))
            {
                Status = EFI_BUFFER_TOO_SMALL;
                *PayloadHeaderSize = sizeof(QPAYLOAD_METADATA_HEADER_V3);
                goto FwFindMetadataExit;
            }

            if(ffsBufferSize >= sizeof(QPAYLOAD_METADATA_HEADER_V3))
            {
                DEBUG((EFI_D_WARN,"    V3 payload found\n"));
                if (NULL != PayloadHeader)
                {
                    CopyMem((VOID*)PayloadHeader,ffsFileBuffer,sizeof(QPAYLOAD_METADATA_HEADER_V3));
                }
            }
            else
            {
                Status = EFI_BAD_BUFFER_SIZE ;
                *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
                *QcResultFlag = QCFW_UPDATE_ERR_METADATA_BAD_V3_HEADER_SIZE;
                DEBUG_ERR_LABEL("    Bad metadata V3  header size\n",FwFindMetadataExit);
            }
            break;
    }

    if (NULL != FileBuffer)
    {
        *FileBuffer = ffsFileBuffer;
    }

    if (NULL != BufferSize)
    {
        *BufferSize = ffsBufferSize;
    }

    return EFI_SUCCESS;

FwFindMetadataExit:
    if (NULL != FileBuffer)
    {
        *FileBuffer = NULL;
    }

    return Status;
}

/**
 * Function to get partition handles
 *
 * @param
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntrySetPartitionHandles(
    IN VOID                      *Image,
    IN UINTN                      ImageSize,
    IN OUT QPAYLOAD_FW_ENTRY*     FwEntry
    )
{
    EFI_STATUS                     Status         = EFI_SUCCESS;
    EFI_HANDLE                     hTemp          = NULL;
    QCOM_FW_UPDATE_STATE_VARIABLES State          = FW_STATE_NOT_IN_PROGRESS;
    EFI_GUID                       BOOT_GUID;
    EFI_GUID                       UnusedVariable;
    EFI_GUID                       DiskTypeGuid;
    GPT_TABLE_STATE                GptState;

    if((FwEntry == NULL) ||
       (Image == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    /* get source handle */
    Status = FwUpdateGetFvBufferHandle(Image,ImageSize,&FVBufferHandle);
    HANDLE_ERROR_LABEL(Status,FwUpdateGetFvBufferHandle,FwEntrySetPartitionHandlesExit);

    FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_SOURCE] = &FVBufferHandle;

    /* get original dest handle */
    switch(FwEntry->Metadata.UpdateType)
    {
    case FWENTRY_UPDATE_TYPE_PARTITION:
    case FWENTRY_UPDATE_TYPE_SSD:

        Status = FwUpdateGetPartitionHandle(&hTemp,
                                          FwEntry->Metadata.UpdatePath.DiskPartitionType,
                                          &(FwEntry->Metadata.UpdatePath.PartitionTypeGUID),
                                          FwEntry->Metadata.UpdatePath.PartitionName,
                                          &gEfiBlockIoProtocolGuid);
        HANDLE_ERROR_LABEL(Status,FwUpdateGetPartitionHandle,FwEntrySetPartitionHandlesExit);

        break;

    case FWENTRY_UPDATE_TYPE_FAT_FILE:

        Status = FwUpdateGetPartitionHandle(&hTemp,
                                          FwEntry->Metadata.UpdatePath.DiskPartitionType,
                                          &(FwEntry->Metadata.UpdatePath.PartitionTypeGUID),
                                          FwEntry->Metadata.UpdatePath.PartitionName,
                                          &gEfiSimpleFileSystemProtocolGuid);
        HANDLE_ERROR_LABEL(Status,FwUpdateGetPartitionHandle,FwEntrySetPartitionHandlesExit);

        break;

    case FWENTRY_UPDATE_TYPE_DPP_OEM:
    case FWENTRY_UPDATE_TYPE_DPP_QCOM:
    case FWENTRY_UPDATE_TYPE_OPM_PRIV_KEY:
    case FWENTRY_UPDATE_TYPE_FWCLASS_GUID:
        hTemp = NULL;
        break;
    default:
        /* Unknown update type should be allowed if double update is needed */
        if(bDoubleUpdateNeeded)
        {
            hTemp = NULL;
            break;
        }
        else
        {
            Status = EFI_INVALID_PARAMETER;
            DEBUG((EFI_D_ERROR,"\nUnknown UpdateType\n"));
            goto FwEntrySetPartitionHandlesExit;
        }
    }

    FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST] = hTemp;

    /* get original backup handle */
    switch(FwEntry->Metadata.BackupType)
    {
    case FWENTRY_BACKUP_TYPE_PARTITION:
        Status =FwUpdateGetPartitionHandle(&hTemp,
                                         FwEntry->Metadata.BackupPath.DiskPartitionType,
                                         &(FwEntry->Metadata.BackupPath.PartitionTypeGUID),
                                         FwEntry->Metadata.BackupPath.PartitionName,
                                         &gEfiBlockIoProtocolGuid);
        HANDLE_ERROR_LABEL(Status,FwUpdateGetPartitionHandle,FwEntrySetPartitionHandlesExit);

        break;
    case FWENTRY_BACKUP_TYPE_FAT_FILE:
        Status =FwUpdateGetPartitionHandle(&hTemp,
                                         FwEntry->Metadata.BackupPath.DiskPartitionType,
                                         &(FwEntry->Metadata.BackupPath.PartitionTypeGUID),
                                         FwEntry->Metadata.BackupPath.PartitionName,
                                         &gEfiSimpleFileSystemProtocolGuid);
        HANDLE_ERROR_LABEL(Status,FwUpdateGetPartitionHandle,FwEntrySetPartitionHandlesExit);

        break;

    default:
         /*Unknown update type should be allowed if double update is needed*/
        if(bDoubleUpdateNeeded)
        {
            hTemp = NULL;
            break;
        }
        else
        {
            Status = EFI_INVALID_PARAMETER;
            DEBUG((EFI_D_ERROR,"\nUnknown BackupType\n"));
            goto FwEntrySetPartitionHandlesExit;
        }
    }

    FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP] = hTemp;

    /* If booting from UFS device, the destination partition handler have to always be the
     * non-bootable partition handler between LUN1 and LUN2.
     */
    if((FwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) &&
       boot_from_ufs())
    {
        Status = GetPartitionGUIDs(&BOOT_GUID, &UnusedVariable);
        HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, FwEntrySetPartitionHandlesExit);

        /* Check if destination partition GUID is the same as boot partition GUID */
        if (CompareGuid((FwEntryDiskPartitionGuids[FwEntry->Metadata.UpdatePath.DiskPartitionType]), &BOOT_GUID))
        {
            hTemp = FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP];

            /* Don't really need the backup partition handler for UFS update
             * but is copied for consistency
             */
            FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP]
                = FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST];

            FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST] = hTemp;
        }
    }

    /* If both update and back type are partition and EMMC/LUN type of update and
       backup path are the same , and the state of State Machine is equal
       to FW_STATE_BACKUP_DONE or onward, and the GPT table has been
       switched, then exchange the destition handle and backup handle.
     */
    Status = GetState(&State);
    HANDLE_ERROR_LABEL(Status,GetState,FwEntrySetPartitionHandlesExit);

    if((FwEntry->Metadata.UpdatePath.DiskPartitionType == FwEntry->Metadata.BackupPath.DiskPartitionType) &&
       (FwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) &&
       (FwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION) &&
       (State >= FW_STATE_BACKUP_DONE))
    {

        Status = FwUpdateGetDiskPartitionTypeGuid(FwEntry->Metadata.UpdatePath.DiskPartitionType,
                                                  &DiskTypeGuid);
        HANDLE_ERROR_LABEL(Status,FwUpdateGetDiskPartitionTypeGuid,FwEntrySetPartitionHandlesExit);

        Status = CmpGPTTable(BACKUP_GPT_TABLE,DiskTypeGuid,&GptState);
        HANDLE_ERROR_LABEL(Status,CmpGPTTable,FwEntrySetPartitionHandlesExit);

        if(GptState == SWITCHED_TABLE)
        {
             hTemp = FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP];

             FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_BACKUP]
                      = FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST];

             FwEntry->Handle[FWENTRY_OPERATION_PATH_TYPE_DEST] = hTemp;
        }
    }
FwEntrySetPartitionHandlesExit:

    return Status;
}

/**
 * Function to decrypt a payload v3 fw entry
 *
 * @param FwEntry - payload fw entry

 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryCrypto(
    IN QPAYLOAD_FW_ENTRY* FwEntry,
    OUT UINT32           *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{
    EFI_STATUS Status           = EFI_SUCCESS;
    VOID       *SrcBuf          = NULL;
    UINTN      SrcBufSize       = 0;
    VOID       *DecryptedBuf    = NULL;
    UINTN      DecryptedBufSize = 0;

    if((FwEntry == NULL) ||
       (MsftAttemptStatus == NULL) ||
       (QcResultFlag == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_INIT;

    FwEntry->IsEncrypted = FALSE;

    if(FwEntry->Metadata.Operation != FWENTRY_OPERATION_TYPE_UPDATE)
    {
        return EFI_SUCCESS;
    }
    Status = FwUpdateReadEntryFromFV(FWENTRY_OPERATION_PATH_TYPE_SOURCE,
                                     FwEntry,
                                     &SrcBuf,
                                     &SrcBufSize);
    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_READ_ENTRY_FROM_FV;
        HANDLE_ERROR_LABEL(Status,FwUpdateReadEntryFromFV,FwEntryCryptoExit);
    }

    switch (FwEntry->Metadata.UpdateType)
    {
    case FWENTRY_UPDATE_TYPE_PARTITION:
    case FWENTRY_UPDATE_TYPE_FAT_FILE:
    case FWENTRY_UPDATE_TYPE_DPP_QCOM:
    case FWENTRY_UPDATE_TYPE_DPP_OEM:
    case FWENTRY_UPDATE_TYPE_FWCLASS_GUID:
    /* SSD decryption is no longer supported */
    //Status = FwCryptoSsdDecrypt(SrcBuf,SrcBufSize,&DecryptedBuf,&DecryptedBufSize);
    //if (EFI_ERROR(Status))
    //{
    //  *QcResultFlag = QCFW_UPDATE_ERR_CRYPTO_SSD_DECRYPT;
    //  HANDLE_ERROR_LABEL(Status,FwCryptoSsdDecrypt,FwEntryCryptoExit);
    //}

    break;

    case FWENTRY_UPDATE_TYPE_OPM_PRIV_KEY:

        Status = FWCryptoOpmPrivKey(SrcBuf,SrcBufSize,&DecryptedBuf,&DecryptedBufSize);
        if (EFI_ERROR(Status))
        {
            *QcResultFlag = QCFW_UPDATE_ERR_CRYPTO_OPM_PRIV_KEY_CRYPTO;
            HANDLE_ERROR_LABEL(Status,FWCryptoOpmPrivKey,FwEntryCryptoExit);
        }
        FwEntry->IsEncrypted = TRUE;
        break;

    case FWENTRY_UPDATE_TYPE_SSD:

        Status = FwCryptoMsmEncrypt(SrcBuf,SrcBufSize,&DecryptedBuf,&DecryptedBufSize);
        if (EFI_ERROR(Status))
        {
            *QcResultFlag = QCFW_UPDATE_ERR_CRYPTO_MSM_KEY_ENCRYPT;
            HANDLE_ERROR_LABEL(Status,FwCryptoMsmEncrypt,FwEntryCryptoExit);
        }
        FwEntry->IsEncrypted = TRUE;
        break;

    default:
      /*Unknown update type should be allowed if double update is needed*/
        if(bDoubleUpdateNeeded)
        {
            goto FwEntryCryptoExit;
        }
        else
        {
            Status = EFI_INVALID_PARAMETER;
            *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_UNKNOWN_UPDATE_TYPE;
            DEBUG_ERR_LABEL("Unknown UpdateType\n",FwEntryCryptoExit);
        }
    }
  
    if(SrcBuf != NULL)
    {
        gBS->FreePool(SrcBuf);
        SrcBuf = NULL;
    }

    FwEntry->DecryptedFwEntryBuffer = DecryptedBuf;
    FwEntry->DecryptedFwEntrySize = DecryptedBufSize;

    return EFI_SUCCESS;

FwEntryCryptoExit:

    if(SrcBuf != NULL)
    {
        gBS->FreePool(SrcBuf);
        SrcBuf = NULL;
    }
    if(DecryptedBuf != NULL)
    {
        UncachedFreePool(DecryptedBuf);
        DecryptedBuf = NULL;
    }
    return Status;
}

/**
 *   Combination masking for Dpp fields
 *
 * @param DevPath      - Device path
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryDevicePathMaskingForDpp(
  IN OUT FWENTRY_DEVICE_PATH *DevPath
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if(DevPath == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    DevPath->DiskPartitionType = FWENTRY_DISK_PARTITION_DONT_CARE;
    ZeroMem((VOID*)DevPath->PartitionName, PARTITION_NAME_MAX_CHAR16_SIZE * sizeof(CHAR16));
    ZeroMem((VOID*)&DevPath->PartitionTypeGUID, sizeof(EFI_GUID));

    return Status;
};

/**
 *   Combination masking for FAT fields, currently all fields are required
 *
 * @param DevPath      - Device path
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryDevicePathMaskingForFat(
  IN OUT FWENTRY_DEVICE_PATH *DevPath
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(DevPath == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  return Status;
};


/**
 *   Combination masking for partition fields
 *
 * @param DevPath      - Device path
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryDevicePathMaskingForPartition(
    IN OUT FWENTRY_DEVICE_PATH *DevPath
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(DevPath == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem(DevPath->FileName, FILE_PATH_MAX_SIZE * sizeof(CHAR16));

  return Status;
};

/**
*   Function to check a payload v3 fw entry Flash Type value
*
* @param mData       - metadata
* @param bEntryGood  - result of validation
*
* @return : EFI_STATUS
*/
EFI_STATUS
FwEntryFlashTypeChecking(
    IN FWENTRY_METADATA *mData,
    OUT BOOLEAN         *bEntryGood
)
{
  //Partitions enum's offset for eMMC and UFS
  const FWENTRY_DISK_PARTITION_TYPE FWENTRY_Enum_EMMC_DISK_PARTITION_MAX  = FWENTRY_UFS_LUN0;
  const FWENTRY_DISK_PARTITION_TYPE FWENTRY_Enum_UFS_DISK_PARTITION_FIRST = FWENTRY_UFS_LUN0;
  const FWENTRY_DISK_PARTITION_TYPE FWENTRY_Enum_UFS_DISK_PARTITION_MAX   = FWENTRY_DISK_PARTITION_MAX;

  EFI_STATUS Status = EFI_SUCCESS;

  if((mData == NULL) || (bEntryGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bEntryGood = FALSE;

  /*FlashType check only validates if FW's Flash Type correspond to device's Flash Type (eMMC or UFS) */

  if (boot_from_ufs())
  {
    if (((mData->UpdatePath.DiskPartitionType >= FWENTRY_Enum_UFS_DISK_PARTITION_FIRST) &&
         (mData->UpdatePath.DiskPartitionType <  FWENTRY_Enum_UFS_DISK_PARTITION_MAX)   &&
         (mData->BackupPath.DiskPartitionType >= FWENTRY_Enum_UFS_DISK_PARTITION_FIRST) &&
         (mData->BackupPath.DiskPartitionType <  FWENTRY_Enum_UFS_DISK_PARTITION_MAX)) ||
        (mData->UpdatePath.DiskPartitionType ==  FWENTRY_DISK_PARTITION_DONT_CARE))
    {
      Status = EFI_SUCCESS;
    }
    else
    {
      Status = EFI_INCOMPATIBLE_VERSION;
    }

    if (EFI_ERROR(Status))
    {
      DEBUG_ERR_LABEL("Incompatible Update/Backup Type detected in FW Entry for flash type 'UFS'\n", FwEntryFlashTypeCheckingExit);
    }
  }
  else if (boot_from_emmc())
  {
    if (((mData->UpdatePath.DiskPartitionType <  FWENTRY_Enum_EMMC_DISK_PARTITION_MAX)  &&
         (mData->BackupPath.DiskPartitionType <  FWENTRY_Enum_EMMC_DISK_PARTITION_MAX)) ||
        (mData->UpdatePath.DiskPartitionType ==  FWENTRY_DISK_PARTITION_DONT_CARE))
    {
      Status = EFI_SUCCESS;
    }
    else
    {
      Status = EFI_INCOMPATIBLE_VERSION;
    }

    if (EFI_ERROR(Status))
    {
      DEBUG_ERR_LABEL("Incompatible Update/Backup Type detected in FW Entry for flash type 'eMMC'\n", FwEntryFlashTypeCheckingExit);
    }
  }
  else
  {
    Status = EFI_INCOMPATIBLE_VERSION;
    DEBUG_ERR_LABEL("Could not determine if device is 'UFS' or 'eMMC'\n", FwEntryFlashTypeCheckingExit);
  }

  *bEntryGood = TRUE;
  return EFI_SUCCESS;

FwEntryFlashTypeCheckingExit:

  return Status;
}

/**
 *   Function to check a payload v3 fw entry value
 *
 * @param mData       - metadata
 * @param bEntryGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryCombinationChecking(
    IN FWENTRY_METADATA *mData,
    OUT BOOLEAN         *bEntryGood
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if((mData == NULL) || (bEntryGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bEntryGood = FALSE;

  /*Combination check only validate known OperationType/UpdateType/BackupType */

  if(mData->Operation == FWENTRY_OPERATION_TYPE_UPDATE)
  {
    if((mData->UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) ||
       (mData->UpdateType == FWENTRY_UPDATE_TYPE_SSD))
    {
      Status = FwEntryDevicePathMaskingForPartition(&mData->UpdatePath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForPartition,FwEntryCombinationCheckingExit);
    }
    else if(mData->UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE)
    {
      Status = FwEntryDevicePathMaskingForFat(&mData->UpdatePath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForFat,FwEntryCombinationCheckingExit);
    }
    else if((mData->UpdateType == FWENTRY_UPDATE_TYPE_DPP_QCOM) ||
            (mData->UpdateType == FWENTRY_UPDATE_TYPE_DPP_OEM) ||
            (mData->UpdateType == FWENTRY_UPDATE_TYPE_OPM_PRIV_KEY) ||
            (mData->UpdateType == FWENTRY_UPDATE_TYPE_FWCLASS_GUID))
    {
      Status = FwEntryDevicePathMaskingForDpp(&mData->UpdatePath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForDpp,FwEntryCombinationCheckingExit);
    }
    else
    {
      if(!bDoubleUpdateNeeded)
      {
        Status = EFI_UNSUPPORTED;
        DEBUG_ERR_LABEL("Unsupported OperationType + UpdateType",FwEntryCombinationCheckingExit);
      }
    }

    if(mData->BackupType== FWENTRY_BACKUP_TYPE_PARTITION)
    {
      Status = FwEntryDevicePathMaskingForPartition(&mData->BackupPath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForPartition,FwEntryCombinationCheckingExit);
    }
    else if(mData->BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE)
    {
      Status = FwEntryDevicePathMaskingForFat(&mData->BackupPath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForFat,FwEntryCombinationCheckingExit);
    }
    else
    {
      if(!bDoubleUpdateNeeded)
      {
        Status = EFI_UNSUPPORTED;
        DEBUG_ERR_LABEL("Unsupported OperationType + BackUpType",FwEntryCombinationCheckingExit);
      }
    }
  }
  else if(mData->Operation == FWENTRY_OPERATION_TYPE_DELETE)
  {
    if(mData->UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE)
    {
      Status = FwEntryDevicePathMaskingForFat(&mData->UpdatePath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForFat,FwEntryCombinationCheckingExit);
    }
    else
    {
      if(!bDoubleUpdateNeeded)
      {
        Status = EFI_UNSUPPORTED;
        DEBUG_ERR_LABEL("Unsupported OperationType + UpdateType",FwEntryCombinationCheckingExit);
      }
    }

    if(mData->BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE)
    {
      Status = FwEntryDevicePathMaskingForFat(&mData->BackupPath);
      HANDLE_ERROR_LABEL(Status,FwEntryDevicePathMaskingForFat,FwEntryCombinationCheckingExit);
    }
    else
    {
      if(!bDoubleUpdateNeeded)
      {
        Status = EFI_UNSUPPORTED;
        DEBUG_ERR_LABEL("Unsupported OperationType + BackupType",FwEntryCombinationCheckingExit);
      }
    }
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("Unsupported OperationType",FwEntryCombinationCheckingExit);
  }

  *bEntryGood = TRUE;
  return EFI_SUCCESS;

FwEntryCombinationCheckingExit:

  return Status;
};

/**
 *   Function to check a payload v3 fw entry value
 *
 * @param mData       - metadata
 * @param bEntryGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryValueChecking(
    IN FWENTRY_METADATA *mData,
    OUT BOOLEAN         *bEntryGood
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if((mData == NULL) || (bEntryGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bEntryGood = FALSE;

  if(mData->Operation >= FWENTRY_OPERATION_TYPE_MAX)
  {
    Status = EFI_UNSUPPORTED;
    DEBUG_ERR_LABEL("\nUnknown Operation type\n",FwEntryValueCheckingExit);
  }

  if((mData->UpdateType >= FWENTRY_UPDATE_TYPE_MAX))
  {
    if(!bDoubleUpdateNeeded)
    {
      Status = EFI_UNSUPPORTED;
      DEBUG_ERR_LABEL("\nUnknown UpdateType\n",FwEntryValueCheckingExit);
    }
  }

  if((mData->BackupType >= FWENTRY_BACKUP_TYPE_MAX))
  {
    if(!bDoubleUpdateNeeded)
    {
      Status = EFI_UNSUPPORTED;
      DEBUG_ERR_LABEL("\nUnknown BackupType\n",FwEntryValueCheckingExit);
    }
  }

  if((mData->UpdatePath.DiskPartitionType >= FWENTRY_DISK_PARTITION_MAX))
  {
    if(!bDoubleUpdateNeeded)
    {
      Status = EFI_UNSUPPORTED;
      DEBUG_ERR_LABEL("\nUnknown Update DiskType\n",FwEntryValueCheckingExit);
    }
  }

  if((mData->BackupPath.DiskPartitionType >= FWENTRY_DISK_PARTITION_MAX))
  {
    if(!bDoubleUpdateNeeded)
    {
      Status = EFI_UNSUPPORTED;
      DEBUG_ERR_LABEL("\nUnknown Backup DiskType\n",FwEntryValueCheckingExit);
    }
  }

  *bEntryGood = TRUE;
  return EFI_SUCCESS;

FwEntryValueCheckingExit:

  return Status;
};


/**
 *   Function to validate a payload v3 fw entry
 *
 * @param mData      - metadata
 * @param bEntryGood - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryValidation(
    IN FWENTRY_METADATA *mData,
    OUT BOOLEAN         *bEntryGood,
    OUT UINT32          *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if((mData == NULL) ||
     (bEntryGood == NULL) ||
     (MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))

  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;
  *bEntryGood = FALSE;

  Status = FwEntryValueChecking(mData,bEntryGood);
  if (EFI_ERROR(Status))
  {
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_VALUE_CHECKING;
    HANDLE_ERROR_LABEL(Status,FwEntryValueChecking,FwEntryValidationExit);
  }

  if(!*bEntryGood)
  {
    Status = EFI_UNSUPPORTED;
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_BAD_VALUE;
    DEBUG_ERR_LABEL("\nFwEntry value validation failed\n",FwEntryValidationExit);
  }

  Status = FwEntryCombinationChecking(mData,bEntryGood);
  if (EFI_ERROR(Status))
  {
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_COMBINATION_CKECKING;
    HANDLE_ERROR_LABEL(Status,FwEntryCombinationChecking,FwEntryValidationExit);
  }

  if(!*bEntryGood)
  {
    Status = EFI_UNSUPPORTED;
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_BAD_COMBINATION;
    DEBUG_ERR_LABEL("\nFwEntry Combination validation failed\n",FwEntryValidationExit);
  }

  Status = FwEntryFlashTypeChecking(mData, bEntryGood);
  if (EFI_ERROR(Status))
  {
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_FLASHTYPE_CHECKING;
    HANDLE_ERROR_LABEL(Status, FwEntryFlashTypeChecking, FwEntryValidationExit);
  }

  if (!*bEntryGood)
  {
    Status = EFI_UNSUPPORTED;
    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_BAD_FLASHTYPE;
    DEBUG_ERR_LABEL("\nFwEntry FlashType validation failed\n", FwEntryValidationExit);
  }

  return EFI_SUCCESS;

FwEntryValidationExit:

  return Status;
};

/**
 *   Function to check if a Fat Update/Backup DevicePath is  execlusive in
 *   a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListFatDevicePathExclusiveChecking(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *bListGood
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN               NodeCount       = 0;
  UINTN               TarGetNodeCount = 0;
  FW_LIST_NODE_TYPE   *TargetNode    = NULL;
  FW_LIST_NODE_TYPE   *HeadNode      = NULL;
  FW_LIST_NODE_TYPE   *BaseNode      = NULL;
  QPAYLOAD_FW_ENTRY   *BaseFwEntry   = NULL;
  QPAYLOAD_FW_ENTRY   *TargetFwEntry = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList   = NULL;
  UINTN               i              = 0;

  if((List == NULL) || (bListGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bListGood = FALSE;
  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,FwListFatDevicePathExclusiveCheckingExit);

  Status = LinkedListGetHeadNode(FwEntryList,&HeadNode);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwListFatDevicePathExclusiveCheckingExit);

  if (HeadNode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwListFatDevicePathExclusiveCheckingExit);

  BaseNode = HeadNode;

  for (i = 0; i < NodeCount; i++)
  {
    if (BaseNode != NULL)
    {
      BaseFwEntry = (QPAYLOAD_FW_ENTRY*)BaseNode->Data;

      /*case 1: BaseFwEntry's updatePath VS BaseFwEntry's BackupPath*/
      if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE) &&
         (BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE))
      {
        if(CompareMem(&BaseFwEntry->Metadata.UpdatePath,
                      &BaseFwEntry->Metadata.BackupPath,
                      sizeof(FWENTRY_DEVICE_PATH)) == 0)
        {
          *bListGood = FALSE;
          goto FwListFatDevicePathExclusiveCheckingExit;
        }
      }

      TargetNode = BaseNode->Next;

      while((TargetNode != NULL) && (TarGetNodeCount <= (NodeCount)))
      {
        TargetFwEntry = (QPAYLOAD_FW_ENTRY*)TargetNode->Data;

        /*case 2: BaseFwEntry's updatePath VS TargetFwEntry's UpdatePath*/
        if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE) &&
           (TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE))
        {
          if(CompareMem(&BaseFwEntry->Metadata.UpdatePath,
                        &TargetFwEntry->Metadata.UpdatePath,
                        sizeof(FWENTRY_DEVICE_PATH)) == 0)
          {
            *bListGood = FALSE;
            goto FwListFatDevicePathExclusiveCheckingExit;
          }
        }

        /*case 3: BaseFwEntry's updatePath VS TargetFwEntry's BackupPath*/
        if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE) &&
           (TargetFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE))
        {
          if(CompareMem(&BaseFwEntry->Metadata.UpdatePath,
                        &TargetFwEntry->Metadata.BackupPath,
                        sizeof(FWENTRY_DEVICE_PATH)) == 0)
          {
            *bListGood = FALSE;
            goto FwListFatDevicePathExclusiveCheckingExit;
          }
        }

        /*case 4: BaseFwEntry's BackupPath VS TargetFwEntry's UpdatePath*/
        if((BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE) &&
           (TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FAT_FILE))
        {
          if(CompareMem(&BaseFwEntry->Metadata.BackupPath,
                        &TargetFwEntry->Metadata.UpdatePath,
                        sizeof(FWENTRY_DEVICE_PATH)) == 0)
          {
            *bListGood = FALSE;
            goto FwListFatDevicePathExclusiveCheckingExit;
          }
        }
        /*case 5: BaseFwEntry's BackupPath VS TargetFwEntry's BackupPath*/
        if((BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE) &&
           (TargetFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_FAT_FILE))
        {
          if(CompareMem(&BaseFwEntry->Metadata.BackupPath,
                        &TargetFwEntry->Metadata.BackupPath,
                        sizeof(FWENTRY_DEVICE_PATH)) == 0)
          {
            *bListGood = FALSE;
            goto FwListFatDevicePathExclusiveCheckingExit;
          }
        }
        TargetNode = TargetNode->Next;
        TarGetNodeCount++;
      }
    }
    else
    {
      break;
    }
    BaseNode = BaseNode->Next;
  }

  *bListGood = TRUE;

  return EFI_SUCCESS;


FwListFatDevicePathExclusiveCheckingExit:

  return Status;

}


/**
 *   Function to check if a DppItem is execlusive in
 *   a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListOemDppItemNameExclusiveChecking(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *bListGood
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN               NodeCount       = 0;
  UINTN               TarGetNodeCount = 0;
  FW_LIST_NODE_TYPE   *TargetNode    = NULL;
  FW_LIST_NODE_TYPE   *HeadNode      = NULL;
  FW_LIST_NODE_TYPE   *BaseNode      = NULL;
  QPAYLOAD_FW_ENTRY   *BaseFwEntry   = NULL;
  QPAYLOAD_FW_ENTRY   *TargetFwEntry = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList   = NULL;
  UINTN               i              = 0;

  if((List == NULL) || (bListGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bListGood = FALSE;
  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,FwListOemDppItemNameExclusiveCheckingExit);

  Status = LinkedListGetHeadNode(FwEntryList,&HeadNode);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwListOemDppItemNameExclusiveCheckingExit);

  if (HeadNode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwListOemDppItemNameExclusiveCheckingExit);

  BaseNode = HeadNode;

  for (i = 0; i < NodeCount; i++)
  {
    if (BaseNode != NULL)
    {
      BaseFwEntry = (QPAYLOAD_FW_ENTRY*)BaseNode->Data;

      if(BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_DPP_OEM)
      {
        TargetNode = BaseNode->Next;

        while((TargetNode != NULL) && (TarGetNodeCount <= (NodeCount)))
        {
          TargetFwEntry = (QPAYLOAD_FW_ENTRY*)TargetNode->Data;

          if(TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_DPP_OEM)
          {
            if(CompareMem(BaseFwEntry->Metadata.UpdatePath.FileName,
                          TargetFwEntry->Metadata.UpdatePath.FileName,
                          FILE_PATH_MAX_SIZE) == 0)
            {
              *bListGood = FALSE;
              goto FwListOemDppItemNameExclusiveCheckingExit;
            }
          }

          TargetNode = TargetNode->Next;
          TarGetNodeCount++;
        }
      }
      else
      {
        BaseNode = BaseNode->Next;
        continue;
      }
    }
    else
    {
      break;
    }

  }

  *bListGood = TRUE;
  return EFI_SUCCESS;

FwListOemDppItemNameExclusiveCheckingExit:

  return Status;

}

/**
 *   Function to check if a DppItem is execlusive in
 *   a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListQcomDppItemNameExclusiveChecking(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *bListGood
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN               NodeCount       = 0;
  UINTN               TarGetNodeCount = 0;
  FW_LIST_NODE_TYPE   *TargetNode    = NULL;
  FW_LIST_NODE_TYPE   *HeadNode      = NULL;
  FW_LIST_NODE_TYPE   *BaseNode      = NULL;
  QPAYLOAD_FW_ENTRY   *BaseFwEntry   = NULL;
  QPAYLOAD_FW_ENTRY   *TargetFwEntry = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList   = NULL;
  UINTN               i              = 0;

  if((List == NULL) || (bListGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bListGood = FALSE;
  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,FwListQcomDppItemNameExclusiveCheckingExit);

  Status = LinkedListGetHeadNode(FwEntryList,&HeadNode);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwListQcomDppItemNameExclusiveCheckingExit);

  if (HeadNode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwListQcomDppItemNameExclusiveCheckingExit);

  BaseNode = HeadNode;

  for (i = 0; i < NodeCount; i++)
  {
    if (BaseNode != NULL)
    {
      BaseFwEntry = (QPAYLOAD_FW_ENTRY*)BaseNode->Data;

      if(BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_DPP_QCOM)
      {
        TargetNode = BaseNode->Next;

        while((TargetNode != NULL) && (TarGetNodeCount <= (NodeCount)))
        {
          TargetFwEntry = (QPAYLOAD_FW_ENTRY*)TargetNode->Data;

          if(TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_DPP_QCOM)
          {
            if(CompareMem(BaseFwEntry->Metadata.UpdatePath.FileName,
                          TargetFwEntry->Metadata.UpdatePath.FileName,
                          FILE_PATH_MAX_SIZE) == 0)
            {
              *bListGood = FALSE;
              goto FwListQcomDppItemNameExclusiveCheckingExit;
            }
          }

          TargetNode = TargetNode->Next;
          TarGetNodeCount++;
        }
      }
      else
      {
        BaseNode = BaseNode->Next;
        continue;
      }
    }
    else
    {
      break;
    }

  }

  *bListGood = TRUE;
  return EFI_SUCCESS;

FwListQcomDppItemNameExclusiveCheckingExit:

  return Status;

}

/**
 *   Function to check if a partition Update/Backup DevicePath is execlusive in
 *   a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListPartitionDevicePathExclusiveChecking(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *bListGood
    )
{
  /*Emmc Type +Partition Type guid + Partition name should be exclusive in
   Update/Backup path if updateType/BackupType is partition.*/
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN               NodeCount       = 0;
  UINTN               TarGetNodeCount = 0;
  FW_LIST_NODE_TYPE   *TargetNode    = NULL;
  FW_LIST_NODE_TYPE   *HeadNode      = NULL;
  FW_LIST_NODE_TYPE   *BaseNode      = NULL;
  QPAYLOAD_FW_ENTRY   *BaseFwEntry   = NULL;
  QPAYLOAD_FW_ENTRY   *TargetFwEntry = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList   = NULL;
  UINTN               i              = 0;

  if((List == NULL) || (bListGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *bListGood = FALSE;
  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,FwListPartitionDevicePathExclusiveCheckingExit);

  Status = LinkedListGetHeadNode(FwEntryList,&HeadNode);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwListPartitionDevicePathExclusiveCheckingExit);

  if (HeadNode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwListPartitionDevicePathExclusiveCheckingExit);

  BaseNode = HeadNode;

  for (i = 0; i < NodeCount; i++)
  {
    if (BaseNode != NULL)
    {
      BaseFwEntry = (QPAYLOAD_FW_ENTRY*)BaseNode->Data;

      /*case 1: BaseFwEntry's updatePath VS BaseFwEntry's BackupPath*/
      if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) &&
         (BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION))
      {
        if((BaseFwEntry->Metadata.UpdatePath.DiskPartitionType ==
            BaseFwEntry->Metadata.BackupPath.DiskPartitionType) &&
           (CompareGuid((GUID*)&BaseFwEntry->Metadata.UpdatePath.PartitionTypeGUID,
                        (GUID*)&BaseFwEntry->Metadata.BackupPath.PartitionTypeGUID)) &&
           (CompareMem(BaseFwEntry->Metadata.UpdatePath.PartitionName,
                       BaseFwEntry->Metadata.BackupPath.PartitionName,
                       PARTITION_NAME_MAX_CHAR16_SIZE*sizeof(CHAR16)) == 0))
        {
          *bListGood = FALSE;
          goto FwListPartitionDevicePathExclusiveCheckingExit;
        }
      }

      TargetNode = BaseNode->Next;

      while((TargetNode != NULL) && (TarGetNodeCount <= (NodeCount)))
      {
        TargetFwEntry = (QPAYLOAD_FW_ENTRY*)TargetNode->Data;

        /*case 2: BaseFwEntry's updatePath VS TargetFwEntry's UpdatePath*/
        if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) &&
           (TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION))
        {
          if((BaseFwEntry->Metadata.UpdatePath.DiskPartitionType ==
              TargetFwEntry->Metadata.UpdatePath.DiskPartitionType) &&
             (CompareGuid((GUID*)&BaseFwEntry->Metadata.UpdatePath.PartitionTypeGUID,
                          (GUID*)&TargetFwEntry->Metadata.UpdatePath.PartitionTypeGUID)) &&
             (CompareMem(BaseFwEntry->Metadata.UpdatePath.PartitionName,
                         TargetFwEntry->Metadata.UpdatePath.PartitionName,
                         PARTITION_NAME_MAX_CHAR16_SIZE*sizeof(CHAR16)) == 0))
          {
            *bListGood = FALSE;
            goto FwListPartitionDevicePathExclusiveCheckingExit;
          }
        }

        /*case 3: BaseFwEntry's updatePath VS TargetFwEntry's BackupPath*/
        if((BaseFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION) &&
           (TargetFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION))
        {
          if((BaseFwEntry->Metadata.UpdatePath.DiskPartitionType ==
              TargetFwEntry->Metadata.BackupPath.DiskPartitionType) &&
             (CompareGuid((GUID*)&BaseFwEntry->Metadata.UpdatePath.PartitionTypeGUID,
                          (GUID*)&TargetFwEntry->Metadata.BackupPath.PartitionTypeGUID)) &&
             (CompareMem(BaseFwEntry->Metadata.UpdatePath.PartitionName,
                         TargetFwEntry->Metadata.BackupPath.PartitionName,
                         PARTITION_NAME_MAX_CHAR16_SIZE*sizeof(CHAR16)) == 0))
          {
            *bListGood = FALSE;
            goto FwListPartitionDevicePathExclusiveCheckingExit;
          }
        }

        /*case 4: BaseFwEntry's BackupPath VS TargetFwEntry's UpdatePath*/
        if((BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION) &&
           (TargetFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_PARTITION))
        {
          if((BaseFwEntry->Metadata.BackupPath.DiskPartitionType ==
              TargetFwEntry->Metadata.UpdatePath.DiskPartitionType) &&
             (CompareGuid((GUID*)&BaseFwEntry->Metadata.BackupPath.PartitionTypeGUID,
                          (GUID*)&TargetFwEntry->Metadata.UpdatePath.PartitionTypeGUID)) &&
             (CompareMem(BaseFwEntry->Metadata.BackupPath.PartitionName,
                         TargetFwEntry->Metadata.UpdatePath.PartitionName,
                         PARTITION_NAME_MAX_CHAR16_SIZE*sizeof(CHAR16)) == 0))
          {
            *bListGood = FALSE;
            goto FwListPartitionDevicePathExclusiveCheckingExit;
          }
        }
        /*case 5: BaseFwEntry's BackupPath VS TargetFwEntry's BackupPath*/
        if((BaseFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION) &&
           (TargetFwEntry->Metadata.BackupType == FWENTRY_BACKUP_TYPE_PARTITION))
        {
          if((BaseFwEntry->Metadata.BackupPath.DiskPartitionType ==
              TargetFwEntry->Metadata.BackupPath.DiskPartitionType) &&
             (CompareGuid((GUID*)&BaseFwEntry->Metadata.BackupPath.PartitionTypeGUID,
                          (GUID*)&TargetFwEntry->Metadata.BackupPath.PartitionTypeGUID)) &&
             (CompareMem(BaseFwEntry->Metadata.BackupPath.PartitionName,
                         TargetFwEntry->Metadata.BackupPath.PartitionName,
                         PARTITION_NAME_MAX_CHAR16_SIZE*sizeof(CHAR16)) == 0))
          {
            *bListGood = FALSE;
            goto FwListPartitionDevicePathExclusiveCheckingExit;
          }
        }
        TargetNode = TargetNode->Next;
        TarGetNodeCount++;
      }
    }
    else
    {
      break;
    }
    BaseNode = BaseNode->Next;
  }

  *bListGood = TRUE;

  return EFI_SUCCESS;

FwListPartitionDevicePathExclusiveCheckingExit:


  return Status;
}

/**
 *   Function to check if SSD is execlusive in a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListSsdExclusiveChecking(
    IN FW_LINKED_LIST_TYPE *List,
    OUT BOOLEAN            *bListGood
    )
{
  EFI_STATUS          Status        = EFI_SUCCESS;
  UINTN               NodeCount     = 0;
  UINTN               UpdateCount   = 0;
  FW_LIST_NODE_TYPE   *Node         = NULL;
  QPAYLOAD_FW_ENTRY   *FwEntry      = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList  = NULL;
  UINTN               i             = 0;
  BOOLEAN             SsdPresent    = FALSE;


  if((List == NULL) || (bListGood == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }
  *bListGood = FALSE;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,FwListSsdExclusiveCheckingExit);

  Status = LinkedListGetHeadNode(FwEntryList,&Node);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,FwListSsdExclusiveCheckingExit);

  if (Node == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,FwListSsdExclusiveCheckingExit);

  for (i = 0; i < NodeCount; i++)
  {
    if (Node != NULL)
    {
      FwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;
      if(FwEntry != NULL)
      {
        if(FwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_SSD)
        {
          SsdPresent = TRUE;

        }
        UpdateCount++;
      }
    }
    else
    {
      break;
    }
    if((UpdateCount > 1) && (SsdPresent == TRUE))
    {
      *bListGood = FALSE;
      goto FwListSsdExclusiveCheckingExit;
    }
    Node = Node->Next;
  }

  *bListGood = TRUE;
  return EFI_SUCCESS;

FwListSsdExclusiveCheckingExit:

  return Status;

}

/**
 *   Function to validate a payload v3 fw entry
 *   linkedlist
 *
 * @param List       - firmware entry linkedlist
 * @param bListGood  - result of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwListValidation(
    IN FW_LINKED_LIST_TYPE       *List,
    OUT BOOLEAN                  *bListGood,
    OUT UINT32                   *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if((List == NULL) ||
       (bListGood == NULL) ||
       (MsftAttemptStatus == NULL) ||
       (QcResultFlag == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_INIT;
    *bListGood = FALSE;

    /*If SSD update is present, it should be exclusive*/
    Status = FwListSsdExclusiveChecking(List, bListGood);

    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_SSD_EXCLUSIVE_CHECKING;
        HANDLE_ERROR_LABEL(Status,FwListSsdExclusiveChecking,FwListValidationExit);
    }

    if(!*bListGood)
    {
        Status = EFI_UNSUPPORTED;
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_SSD_NOT_EXCLUSIVE;
        DEBUG_ERR_LABEL("\n      SSD exclusive validation failed\n",FwListValidationExit);
    }

    /*A partition Update/Backup DevicePath should be execlusive in
      a payload v3 fw entry */

    Status = FwListPartitionDevicePathExclusiveChecking(List, bListGood);
    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_PARTITION_DEVPATH_EXCLUSIVE_CHECKING;
        HANDLE_ERROR_LABEL(Status,FwListPartitionDevicePathExclusiveChecking,FwListValidationExit);
    }

    if(!*bListGood)
    {
        Status = EFI_UNSUPPORTED;
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_PARTITION_DEVPATH_NOT_EXCLUSIVE;
        DEBUG_ERR_LABEL("\n      Partition exclusive validation failed\n",FwListValidationExit);
    }

    /*A QCOM DPP Item name should be execlusive in
      a payload v3 fw entry */

    Status = FwListQcomDppItemNameExclusiveChecking(List, bListGood);
    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_QCOM_DPP_DEVPATH_EXCLUSIVE_CHECKING;
        HANDLE_ERROR_LABEL(Status,FwListQcomDppItemNameExclusiveChecking,FwListValidationExit);
    }

    if(!*bListGood)
    {
        Status = EFI_UNSUPPORTED;
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_QCOM_DPP_DEVPATH_NOT_EXCLUSIVE;
        DEBUG_ERR_LABEL("\n      Qcom DppItem Name exclusive validation failed\n",FwListValidationExit);
    }

    /*A OEM DPP Item name should be execlusive in
       a payload v3 fw entry */

     Status = FwListOemDppItemNameExclusiveChecking(List, bListGood);
     if (EFI_ERROR(Status))
     {
         *QcResultFlag = QCFW_UPDATE_ERR_LIST_OEM_DPP_DEVPATH_EXCLUSIVE_CHECKING;
         HANDLE_ERROR_LABEL(Status,FwListOemDppItemNameExclusiveChecking,FwListValidationExit);
     }

     if(!*bListGood)
     {
         Status = EFI_UNSUPPORTED;
         *QcResultFlag = QCFW_UPDATE_ERR_LIST_OEM_DPP_DEVPATH_NOT_EXCLUSIVE;
         DEBUG_ERR_LABEL("\n      OEM DppItem Name exclusive validation failed\n",FwListValidationExit);
     }

    /*A FAT device path should be execlusive in
       a payload v3 fw entry */

    Status = FwListFatDevicePathExclusiveChecking(List, bListGood);
    if (EFI_ERROR(Status))
    {
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_FAT_DEVPATH_EXCLUSIVE_CHECKING;
        HANDLE_ERROR_LABEL(Status,FwListFatDevicePathExclusiveChecking,FwListValidationExit);
    }

    if(!*bListGood)
    {
        Status = EFI_UNSUPPORTED;
        *QcResultFlag = QCFW_UPDATE_ERR_LIST_FAT_DEVPATH_NOT_EXCLUSIVE;
        DEBUG_ERR_LABEL("\n      Fat device path exclusive validation failed\n",FwListValidationExit);
    }

    return EFI_SUCCESS;

FwListValidationExit:

    return Status;
};

/**
 *. This function should parse V3 payload
 * @param mBuf      - Metadata buffer
 * @param mBufSize - Metadata buffer size
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwPayloadV3Parser(
    IN VOID                      *Image,
    IN UINTN                      ImageSize,
    IN VOID                      *mBuf,
    IN UINTN                      mBufSize,
    OUT UINT32                   *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{
    EFI_STATUS         Status           = EFI_SUCCESS;
    UINT32             TotalFwEntrySize = 0;
    FWENTRY_METADATA   *pMetadataEntry  = NULL;
    BOOLEAN            bEntryGood       = FALSE;
    BOOLEAN            bListGood        = FALSE;
    QPAYLOAD_FW_ENTRY  *pFwEntry        = NULL;
    UINTN              i                = 0;
    UINTN              NodeCount        = 0;
    UINT32             BootLunNumber    = 0;

    if((MsftAttemptStatus == NULL) ||
       (QcResultFlag == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_ERR_METADATA_INVALID_PARAM;

    if((mBuf == NULL) ||
       (Image == NULL) ||
       (mBufSize < sizeof(QPAYLOAD_METADATA_HEADER_V3)))
    {
        return EFI_INVALID_PARAMETER;
    }

    // If there are any stale contents from earlier update, clean it
    if (FwEntryList.Count != 0)
    {
        Status = LinkedListDeleteAllNodes(&FwEntryList);
        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR,"LinkedListDeleteAllNodes returned %r\n", Status));
            goto FwPayloadV3ParserExit;
        }
    }

    /*Basic metadata size validation*/
    QPAYLOAD_METADATA_HEADER_V3 *mDataHeader = (QPAYLOAD_METADATA_HEADER_V3*)mBuf;
    TotalFwEntrySize = mDataHeader->EntryCount * sizeof(FWENTRY_METADATA)
                       + sizeof(QPAYLOAD_METADATA_HEADER_V3);

    if(TotalFwEntrySize != mBufSize)
    {
        *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
        *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT;
        return EFI_INVALID_PARAMETER;
    }

    /*Parse FwEntry metadata value*/
    pMetadataEntry = (FWENTRY_METADATA*)(mDataHeader+1);
    Status = GetBootLUN(&BootLunNumber);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN,"Could not determine BOOT LUN : GetBootLUN returned %r\n", Status));
    }
    else
    {
        DEBUG((EFI_D_ERROR, "\n  ACTIVE LUN: %d\n", BootLunNumber));
    }

    for (i = 0; i < mDataHeader->EntryCount; i++)
    {
        Status = gBS->AllocatePool(EfiBootServicesData, sizeof(QPAYLOAD_FW_ENTRY),(VOID**)&pFwEntry);
        if (EFI_ERROR(Status) || (pFwEntry == NULL))
        {
            *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
            *QcResultFlag = QCFW_UPDATE_ERR_UEFI_API_ALLOCATE_POOL;
            HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,FwPayloadV3ParserExit);
        }
        else
        {
            ZeroMem(pFwEntry,sizeof(QPAYLOAD_FW_ENTRY));
            CopyMem((VOID*)&pFwEntry->Metadata,(VOID*)pMetadataEntry,sizeof(FWENTRY_METADATA));
            pMetadataEntry ++;

            /*Screen Unknown UpdateType/BackupType*/
            if((pFwEntry->Metadata.UpdateType < FWENTRY_UPDATE_TYPE_MAX) &&
               (pFwEntry->Metadata.BackupType < FWENTRY_BACKUP_TYPE_MAX) &&
               (pFwEntry->Metadata.Operation < FWENTRY_OPERATION_TYPE_MAX))
            {
                /*Filling the dest and backup fields for FWCLASS_GUID*/
                if(pFwEntry->Metadata.UpdateType == FWENTRY_UPDATE_TYPE_FWCLASS_GUID)
                {
                  pFwEntry->Metadata.BackupType = FWENTRY_BACKUP_TYPE_FAT_FILE;

                  pFwEntry->Metadata.UpdatePath.DiskPartitionType = FWENTRY_UFS_LUN4;
                  CopyMem(pFwEntry->Metadata.UpdatePath.PartitionName, L"DPP", StrSize(L"DPP"));
                  pFwEntry->Metadata.UpdatePath.PartitionTypeGUID = gEfiDppFatPartitionGuid;
                  CopyMem(pFwEntry->Metadata.UpdatePath.FileName, L"ESRT_SYS_FWClass.PROVISION", StrSize(L"ESRT_SYS_FWClass.PROVISION"));

                  pFwEntry->Metadata.BackupPath.DiskPartitionType = FWENTRY_UFS_LUN4;
                  CopyMem(pFwEntry->Metadata.BackupPath.PartitionName, L"DPP", StrSize(L"DPP"));
                  pFwEntry->Metadata.BackupPath.PartitionTypeGUID = gEfiDppFatPartitionGuid;
                  CopyMem(pFwEntry->Metadata.BackupPath.FileName, L"QCOM\\ESRT_SYS_FWClass.PROVISION.bkp", StrSize(L"QCOM\\ESRT_SYS_FWClass.PROVISION.bkp"));
                }
                /*FwEntry metada validation*/
                Status = FwEntryValidation(&pFwEntry->Metadata,&bEntryGood,MsftAttemptStatus,QcResultFlag);
                HANDLE_ERROR_LABEL(Status,FwEntryValidation,FwPayloadV3ParserExit);

                if(!bEntryGood)
                {
                    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
                    *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT;
                    Status = EFI_UNSUPPORTED;
                    DEBUG_ERR_LABEL("\nFwEntry Validation failed\n",FwPayloadV3ParserExit);
                }

                /*Update FwEntry handles*/
                Status = FwEntrySetPartitionHandles(Image,ImageSize,pFwEntry);
                if (EFI_ERROR(Status))
                {
                    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
                    *QcResultFlag = QCFW_UPDATE_ERR_PARSER_SET_PARTITION_HANDLES;
                    HANDLE_ERROR_LABEL(Status,FwEntrySetPartitionHandles,FwPayloadV3ParserExit);
                }

                /*Cryptography*/
                Status = FwEntryCrypto(pFwEntry,MsftAttemptStatus,QcResultFlag);
                HANDLE_ERROR_LABEL(Status,FwEntryCrypto,FwPayloadV3ParserExit);

                /*Add to fw entry linked list*/
                Status = LinkedListAddNode(&FwEntryList,(VOID*)pFwEntry);
                if (EFI_ERROR(Status))
                {
                    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
                    *QcResultFlag = QCFW_UPDATE_ERR_LIST_ADD_NODE;
                    HANDLE_ERROR_LABEL(Status,LinkedListAddNode,FwPayloadV3ParserExit);
                }
                NodeCount++;
                DEBUG((EFI_D_WARN,"\n    %d FwEntry found...\n",NodeCount));
                FwPrintFwEntry(pFwEntry,EFI_D_WARN);
                /*Set pFwEntry to NULL after adding the node to list so that it avoids
                  double freeing memory at FwPayloadV3ParserExit*/
                pFwEntry = NULL;
            }
            else
            {
                if(bDoubleUpdateNeeded)
                {
                    if(pFwEntry != NULL)
                    {
                        gBS->FreePool(pFwEntry);
                        pFwEntry = NULL;
                    }
                    continue;
                }
                else
                {
                    Status = EFI_UNSUPPORTED;
					*MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
                    *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_UNKNOWN_UPDATE_BACKUP_TYPE;
                    DEBUG_ERR_LABEL("\nUnknown update/backup type\n",FwPayloadV3ParserExit);
                }
            }
        }
    }

    if(FwEntryList.Count == 0)
    {        
        *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
        *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT;
        Status = EFI_UNSUPPORTED;
        DEBUG_WARN_LABEL("\n    No FwEntry found...\n",FwPayloadV3ParserExit);
    }

    DEBUG((EFI_D_WARN,"\n    Validating FwEntry list...\n"));
    Status = FwListValidation(&FwEntryList, &bListGood,MsftAttemptStatus,QcResultFlag);
    HANDLE_ERROR_LABEL(Status,FwListValidation,FwPayloadV3ParserExit);

    if(!bListGood)
    {
        *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
        *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT;
        Status = EFI_UNSUPPORTED;
        DEBUG_ERR_LABEL("\n    FwEntry list validation failed...\n",FwPayloadV3ParserExit);
    }

    DEBUG((EFI_D_WARN,"\n    FwEntry list validated. Totally %d FwEntries found\n",FwEntryList.Count));

    return EFI_SUCCESS;

FwPayloadV3ParserExit:

    if(pFwEntry != NULL)
    {
        DEBUG((EFI_D_ERROR,"Error: Current FwEntry info \n"));
        FwPrintFwEntry(pFwEntry, EFI_D_ERROR);

        gBS->FreePool(pFwEntry);
        pFwEntry = NULL;
    }

	if (FwEntryList.Count != 0)
    {
        DEBUG((EFI_D_ERROR,"Error: List info \n"));
        FwPrintFwList(&FwEntryList, EFI_D_ERROR);
        
        /*Do not check the Status of LinkedListDeleteAllNodes since the overall
        status should be the previous error status*/
        LinkedListDeleteAllNodes(&FwEntryList);
	}

    return Status;
}

/**
 * Payload parser main function
 *
 * @param  MsftAttemptStatus   -Microsoft last attempt status
 * @param  QcResultFlag        -QCFirmwareUpdate result
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwPayloadParserMain(
    IN  VOID                     *Image,
    IN  UINTN                     ImageSize,
    OUT UINT32                   *MsftAttemptStatus,
    OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
    )
{
    EFI_STATUS Status               = EFI_SUCCESS;
    UINT32     Version              = 0;
    VOID      *mBuf                 = NULL;
    UINTN      mBufSize             = 0;
    UINT32     BreakingChangeNumber = 0;
    UINTN      PayLoadHeaderSize    = sizeof(PayLoadHeader);    

    if(Image == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_INIT;

    DEBUG((EFI_D_WARN,"\n  Parsing payload... \n"));
    /* Find payload format version from MetaData */
    Status = FwFindMetadata(Image,ImageSize,&Version,&PayLoadHeader,&PayLoadHeaderSize,&mBuf,&mBufSize,MsftAttemptStatus,QcResultFlag);
    HANDLE_ERROR_LABEL(Status,FwPayloadParserMain,FwPayloadParserMainExit);

    /* Display payload info */
    Status = FwPrintPayloadHeader(&PayLoadHeader, EFI_D_WARN);
    HANDLE_ERROR_LABEL(Status,FwPrintPayloadHeader,FwPayloadParserMainExit);

    //Read the BreakingChangeNumber from disk
    Status = GetBreakingChangeNumber(&BreakingChangeNumber);

    if(Status != EFI_SUCCESS)
    {
        BreakingChangeNumber = 0;
    }
    DEBUG((EFI_D_WARN,"    Current breaking change number is %d\n",BreakingChangeNumber));
    DEBUG((EFI_D_WARN,"    Payload breaking change number is %d\n",PayLoadHeader.BreakingChangeNumber));

    bDoubleUpdateNeeded = (PayLoadHeader.BreakingChangeNumber > BreakingChangeNumber)? TRUE:FALSE;

    if(bDoubleUpdateNeeded)
    {
        DEBUG((EFI_D_WARN,"    Double update required\n\n"));
    }
    else
    {
        DEBUG((EFI_D_WARN,"    Double update not required\n\n"));
    }

    if(Version < 3)
    {
        DEBUG((EFI_D_WARN,"    V1/V2 payload is not supported anymore\n"));
        *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
        *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT;
        Status = EFI_UNSUPPORTED;

        HANDLE_ERROR_LABEL(Status,FwPayloadV1V2Parser,FwPayloadParserMainExit);
    }
    else
    {
        Status = FwPayloadV3Parser(Image,ImageSize,mBuf,mBufSize,MsftAttemptStatus,QcResultFlag);
        HANDLE_ERROR_LABEL(Status,FwPayloadV3Parser,FwPayloadParserMainExit);
    }

FwPayloadParserMainExit:
    mBuf = NULL;

    return Status;
}
