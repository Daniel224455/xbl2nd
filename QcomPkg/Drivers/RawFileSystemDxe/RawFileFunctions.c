/** @file RawFileFunctions.c

Implementation for Qualcomm UEFI RawFileSystem protocol

Copyright (c) 2018 Qualcomm Technologies, Inc. All rights
reserved.

**/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/04/19   rajb    Updated log mask.
08/31/18   rajb    Updated return path log messages when DPP is not formatted.
05/31/18   prema   Fixed issues related to read and write files to raw file system
03/01/18   ltg     Initial revision
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
#include "RawFileHeaderStructures.h"
#include "RawFileFunctions.h"
#include <Protocol/BlockIo.h>

/**
RawFileSystem PROTOCOL interfaces
*/
#include <Protocol/EFIRawFileSystem.h>

/*!
* Partition Header Signature Constants
*/
#define RAWFS_SIGNATURE                 0x53465752 // 'SFWR'
#define RAWFS_REVISION                  0x1000

// Device path information
#define TYPE_MEDIA_DEVICE_PATH     4

// Size info
#define FRAGMENT_REUSE_FREE_SPACE_MAX   10/100  // % max free space allowed for reusing fragmented memory
#define SPINOR_ACCESS_BLOCKSIZE         4096    // 4KB alignment is standard in Windows
#define EXPECTED_NUM_HEADER_BLOCKS          2

#define ALIGN_TO_BLOCK(Size, BlockSize, AlignedSize)    \
  { \
        AlignedSize = Size + (BlockSize - 1);\
        AlignedSize = AlignedSize & ~(BlockSize -1);\
  }

#define UINT32_MAX       0xffffffffu

EFI_STATUS
ReadFileRawFileSystem(
    IN CHAR16 *pReadFileName,
    IN OUT UINTN *FileSize,
    IN OUT UINT8 *pOutBuffer,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    CHAR16 folderName[MAX_FOLDER_NAME_LENGTH] = { 0 };
    CHAR16 fileName[MAX_FILE_NAME_LENGTH] = { 0 };
    UINT32 folderNameLen = 0;
    UINT32 fileNameLen = 0;
    UINT32 fileEntryIndex = 0;
    BOOLEAN foundFile = FALSE;
    UINT8 *pCombinedFSHeaders = NULL;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    PFILE_ENTRY pReadFileEntry = NULL;
    FS_BLOCK FileBlocks;
    UINTN fileLength = 0;
	UINTN fileStartBlockOffset = 0;
    UINT32 fileEndOffsetBlockAligned = 0;
    
    ZeroMem(&FileBlocks, sizeof(FS_BLOCK));

    // verify arguments are valid
    if (!pReadFileName || !FileSize || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem One or more arguments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    // Get file and folder names
    SplitFilePath(pReadFileName, folderName, &folderNameLen, fileName, &fileNameLen);

    // It is OK to not have a folder name, so we need to check if both file and folder are 0
    if (fileNameLen == 0 && folderNameLen == 0)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem pReadFileName is empty!\n"));
        goto Exit;
    }

    // Get file entry headers and find the entry that matches our request
    status = RetrieveFileSystemHeaders(&pCombinedFSHeaders, &pFSHeader, &pDirHeader, &pFileEntries, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        if (status == EFI_VOLUME_CORRUPTED)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem Signature and/or PartitionBlockSize is corrupted. Status = 0x%x.\n", status));
        }
        else if (status == EFI_NOT_FOUND)
        {
            DEBUG((EFI_D_WARN, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem DPP is not formated. Status = 0x%x\n", status));
        }
        else
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem Could not retrieve file system headers. Status = 0x%x.\n", status));
        }

        goto Exit;
    }

    if (!pCombinedFSHeaders || !pFSHeader || !pDirHeader || !pFileEntries)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadFileRawFileSystem RetrieveFileSystemHeaders failed.\n"));
        goto Exit;
    }

    for (UINT16 fileEntryIdx = 0; fileEntryIdx < pDirHeader->MaxFiles; fileEntryIdx++)
    {
        if (StrCmp(pFileEntries[fileEntryIdx].FolderName, folderName) == 0)
        {
            if (StrCmp(pFileEntries[fileEntryIdx].FileName, fileName) == 0)
            {
                fileEntryIndex = fileEntryIdx;
                foundFile = TRUE;
                break;
            }
        }
    }
    if (foundFile == FALSE)
    {
		DEBUG((EFI_D_WARN, "\n[RawFileSystemDxe]FileRead File with name %s not found!\n", fileName));
        status = EFI_NOT_FOUND;
        goto Exit;
    }

    pReadFileEntry = (PFILE_ENTRY)&pFileEntries[fileEntryIndex];

    // If the out buffer is not allocated, populate FileSize parameter with size of entire file. Otherwise, read and copy blocks into the buffer.
    fileLength = pReadFileEntry->UsedSize;

    if (pOutBuffer == NULL)
    {
        *FileSize = fileLength;
    }
    else
    {
        if (fileLength > *FileSize)
        {
            DEBUG((EFI_D_WARN, "\n[RawFileSystemDxe]File does not fit in the given buffer!\n"));
            status = EFI_BUFFER_TOO_SMALL;
            goto Exit;
        }

        // Get the file end offset from start of partition
        fileEndOffsetBlockAligned = (pDirHeader->FileDataStartOffset + pReadFileEntry->Offset + pReadFileEntry->AllocatedSize);
        // Align the end to block end as the file has to consume
        // the whole block as new file cannot start from middle
        // of a block.
        ALIGN_TO_BLOCK(fileEndOffsetBlockAligned, pFSHeader->PartitionBlockSize, fileEndOffsetBlockAligned);
        
        // Get the start block, number of blocks and access size in preparation to read blocks
        FileBlocks.StartBlock = (pDirHeader->FileDataStartOffset + pReadFileEntry->Offset) / pFSHeader->PartitionBlockSize;
        FileBlocks.NumBlocks =  fileEndOffsetBlockAligned / pFSHeader->PartitionBlockSize;
        FileBlocks.NumBlocks -= FileBlocks.StartBlock;
        FileBlocks.BlockSize = pFSHeader->PartitionBlockSize;
        FileBlocks.pBlockData = NULL;

        // Read blocks corresponding to file entry
        status = ReadBlocksFromFileSystem(&FileBlocks, RawFSBlockIo);
        if (EFI_ERROR(status))
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]FileRead failed. Status = 0x%x.\n", status));
            goto Exit;
        }

        // If the length of the output buffer is bigger than the file size, copy the whole file and populate FileSize with size of the file
        if (FileBlocks.pBlockData)
        {
            if (fileLength < *FileSize) {
                *FileSize = fileLength;
            }
            fileStartBlockOffset = (pDirHeader->FileDataStartOffset + pReadFileEntry->Offset) % pFSHeader->PartitionBlockSize;
            CopyMem(pOutBuffer, FileBlocks.pBlockData + fileStartBlockOffset, *FileSize);
            gBS->FreePool(FileBlocks.pBlockData);
            FileBlocks.pBlockData = NULL;
        }
    }

Exit:
    return status;
}

BOOLEAN
VerifyRawFileSystemSignature(
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    PFS_HEADER pFSHeader = NULL;
    UINT8 *pCombinedFSHeaders = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    BOOLEAN verified = FALSE;

    // verify arguments are valid
    if (!RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:VerifyRawFileSystemSignature One or more arguments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    status = RetrieveFileSystemHeaders(&pCombinedFSHeaders, &pFSHeader, &pDirHeader, &pFileEntries, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        if (status == EFI_VOLUME_CORRUPTED)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:VerifyRawFileSystemSignature Signature and/or PartitionBlockSize is corrupted. Status = 0x%x.\n", status));
        }
        else
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:VerifyRawFileSystemSignature Could not retrieve file system headers. Status = 0x%x.\n", status));
        }

        goto Exit;
    }

    if (pFSHeader->Signature == RAWFS_SIGNATURE)
    {
        verified = TRUE;
    }

Exit:
    return verified;
}


EFI_STATUS
FormatRawFileSystem(
    IN UINT32 fsBlockSize,
    IN UINT32 numCombinedHeaderBlocks,
    IN UINT32 fsSizeKb,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    UINT8* pFileSystemBuffer = NULL;
    EFI_STATUS status = EFI_SUCCESS;
    UINT32 FSAndDirHeaderSz = 0;
    UINT32 totalFileEntriesSize = 0;
    UINT32 fileSystemSize = fsSizeKb * 1024;
    UINT32 maxFileEntriesPossible = 0;

    // verify arguments are valid
    if (fsBlockSize == 0 || numCombinedHeaderBlocks == 0 || fsSizeKb == 0 || RawFSBlockIo == NULL)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FormatRawFileSystem One or more arguments are invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    /* TODO: Prem: This is very risky as filesystem with
       large size can causehuge allocation. Should have a logic
       that handle section by section of the file system
       */
    // Allocate and populate partition raw file header
    pFileSystemBuffer = AllocateZeroPool(fileSystemSize);
    if (pFileSystemBuffer == NULL)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FormatRawFileSystem AllocateZeroPool error.\n"));
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    // FS_HEADER
    pFSHeader = (PFS_HEADER)pFileSystemBuffer;
    pFSHeader->FSHeaderSz = sizeof(FS_HEADER);

    pFSHeader->FSBlockSize = fsBlockSize;
    pFSHeader->PartitionBlockSize = RawFSBlockIo->Media->BlockSize;
    pFSHeader->FileSystemSize = fileSystemSize;
    pFSHeader->Revision = RAWFS_REVISION;
    pFSHeader->Signature = RAWFS_SIGNATURE;
    pFSHeader->DirectoryOffset = pFSHeader->FSHeaderSz + 1;
    pFSHeader->AlignedTotalHeaderSz = RawFSBlockIo->Media->BlockSize * numCombinedHeaderBlocks;

    if (pFSHeader->AlignedTotalHeaderSz  > pFSHeader->FileSystemSize)
    {
        status = EFI_BAD_BUFFER_SIZE;
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FormatRawFileSystem Not enough space in the partition!\n"));
        goto Exit;
    }

    // DIRECTORY_HEADER
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pFileSystemBuffer + 1);
    pDirHeader->DirHeaderSize = sizeof(DIRECTORY_HEADER);

    FSAndDirHeaderSz = sizeof(DIRECTORY_HEADER) + sizeof(FS_HEADER);

    // Need to ensure FILE_ENTRY + DIRECTORY_HEADER + FS_HEADER fit within one access block
    maxFileEntriesPossible = (UINT32)((pFSHeader->AlignedTotalHeaderSz - FSAndDirHeaderSz) / sizeof(FILE_ENTRY));
    totalFileEntriesSize = sizeof(FILE_ENTRY) * maxFileEntriesPossible;
    pFSHeader->TotalHeaderSize = FSAndDirHeaderSz + totalFileEntriesSize;

    pDirHeader->TotalFileDataSpace = pFSHeader->FileSystemSize - pFSHeader->AlignedTotalHeaderSz;
    pDirHeader->FileDataStartOffset = pFSHeader->AlignedTotalHeaderSz;
    pDirHeader->MaxFiles = maxFileEntriesPossible;
    pDirHeader->FileEntriesStartOffset = FSAndDirHeaderSz;
    pDirHeader->TotalFileEntriesSize = maxFileEntriesPossible * sizeof(FILE_ENTRY);
    pDirHeader->Signature = RAWFS_SIGNATURE;

    // Write block data
    status = RawFSBlockIo->WriteBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
        (EFI_LBA)0,
        fileSystemSize,
        pFileSystemBuffer);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\nFailed to write block data.\n"));
        goto Exit;
    }

    status = RawFSBlockIo->FlushBlocks(RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\nFailed to flush blocks.\n"));
        goto Exit;
    }

    if (VerifyRawFileSystemSignature(RawFSBlockIo) == FALSE)
    {
        DEBUG((EFI_D_ERROR, "\nVerification failed! Failed to write block data.\n"));
        status = EFI_UNSUPPORTED;
    }


Exit:
    if (pFileSystemBuffer)
    {
        gBS->FreePool(pFileSystemBuffer);
        pFileSystemBuffer = NULL;
    }

    return status;
}


EFI_STATUS
PrepareAddFileWriteBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pNewfileInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_UNSUPPORTED;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;

    // verify arguments are valid
    if (!pCombinedFSHeaders || !pNewfileInfo || !pFileWriteBlocks || !pHeaderWriteBlocks || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareAddFileWriteBlocks One or more arguments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    // Obtain the file and folder name from the FSFilePath

    SplitFilePath(pNewfileInfo->pFSFilePath,
        pNewfileInfo->FolderName, &pNewfileInfo->FolderNameLen,
        pNewfileInfo->FileName, &pNewfileInfo->FileNameLen);

    if (pNewfileInfo->FileNameLen == 0)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareAddFileWriteBlocks File name was not retrieved correctly!\n"));
        status = EFI_NOT_FOUND;
        goto Exit;
    }

    status = GetNewFilePlacementInformation(pCombinedFSHeaders, pNewfileInfo, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareAddFileWriteBlocks GetFileEntryAndDataPosition failed with status: 0x%x.\n", status));
        goto Exit;
    }

    pNewfileInfo->AddFile = TRUE;

    status = PrepareFileSystemBlocks(pCombinedFSHeaders, pNewfileInfo, pFileWriteBlocks, pHeaderWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareAddFileWriteBlocks PrepareFileSystemBlocks failed with status: 0x%x.\n", status));
        goto Exit;
    }
Exit:
    return status;
}

UINT32 GetNumFilesInFS(
    IN UINT8 *pCombinedFSHeaders,
    IN BOOLEAN countFragmented
)
{
    UINT32 numFiles = UINT32_MAX;
    PFILE_ENTRY pFileEntries = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFS_HEADER pFSHeader = NULL;

    UINT32 allocatedSpaceInUse = 0;

    if (!pCombinedFSHeaders)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNumFilesInFS File System Pointer is NULL!\n"));
        goto Exit;
    }

    // Get the FS Header
    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    // Get the Directory Header
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    if (!pFSHeader || !pDirHeader)
    {
        if (!pCombinedFSHeaders)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNumFilesInFS FS Header, Dir Header or File Entry Header is NULL!\n"));
        }
        goto Exit;
    }

    // Get the FileEntries Header
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    if (!pFileEntries)
    {
        if (!pCombinedFSHeaders)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNumFilesInFS FS Header, Dir Header or File Entry Header is NULL!\n"));
        }
        goto Exit;
    }

    allocatedSpaceInUse = 0;
    numFiles = 0;

    for (UINT16 fildEntryIdx = 0; fildEntryIdx < pDirHeader->MaxFiles; fildEntryIdx++)
    {
        // Count valid files
        if (!countFragmented)
        {
            if (pFileEntries[fildEntryIdx].Valid == TRUE)
            {
                allocatedSpaceInUse += pFileEntries[fildEntryIdx].AllocatedSize;
                numFiles++;
            }
        }
        else
        {
            if ((pFileEntries[fildEntryIdx].AllocatedSize > 0) &&
                (pFileEntries[fildEntryIdx].Valid == FALSE))
            {
                numFiles++;
                allocatedSpaceInUse += pFileEntries[fildEntryIdx].AllocatedSize;
            }
        }
    }
Exit:
    return numFiles;
}

EFI_STATUS
RetrieveFileSystemHeaders(
    OUT UINT8 **ppCombinedHeaders,
    OUT PFS_HEADER *ppFSHeader,
    OUT PDIRECTORY_HEADER *ppDirHeader,
    OUT PFILE_ENTRY *ppFileEntries,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    FS_BLOCK combinedHeaderBlocks;
    UINTN numHeaderBlocks = 0;

    ZeroMem(&combinedHeaderBlocks, sizeof(FS_BLOCK));

    // verify arguments are valid
    if ((ppFSHeader == NULL) || (ppFileEntries == NULL) || (ppDirHeader == NULL) || (ppCombinedHeaders == NULL) || (RawFSBlockIo == NULL))
    {
        status = EFI_INVALID_PARAMETER;
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders One of the pointers to headers does not exst, cannot proceed with memory allocation.\n"));
        goto Exit;
    }

    *ppCombinedHeaders = NULL;
    *ppDirHeader = NULL;
    *ppFileEntries = NULL;
    *ppFSHeader = NULL;

    // Read the first 2 blocks only first to determine total size of blocks
    combinedHeaderBlocks.StartBlock = 0;
    combinedHeaderBlocks.NumBlocks = EXPECTED_NUM_HEADER_BLOCKS;
    combinedHeaderBlocks.BlockSize = RawFSBlockIo->Media->BlockSize;

    status = ReadBlocksFromFileSystem(&combinedHeaderBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders Error propagated from ReadBlocksFromFileSystem. status code: 0x%x.\n", status));
        goto Exit;
    }

    *ppFSHeader = (PFS_HEADER)combinedHeaderBlocks.pBlockData;

    if (*ppFSHeader == NULL)
    {
        DEBUG((EFI_D_INFO, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders FS_HEADER was null when retrieved from file system.\n"));
        status = EFI_NOT_FOUND;
        goto Exit;
    }

    // if signature is corrupted, exit with status EFI_VOLUME_CORRUPTED
    if ((*ppFSHeader)->Signature != (UINT32)RAWFS_SIGNATURE)
    {
        status = EFI_VOLUME_CORRUPTED;
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders Raw file system signature is corrupted! status code: 0x%x.\n", status));
        goto Exit;
    }

    // if block size is corrupted, exit with status EFI_VOLUME_CORRUPTED
    if ((*ppFSHeader)->PartitionBlockSize != RawFSBlockIo->Media->BlockSize)
    {
        status = EFI_VOLUME_CORRUPTED;
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders Raw file system block size is corrupted! status code: 0x%x.\n", status));
        goto Exit;
    }

    // check if size is > 2
    numHeaderBlocks = (*ppFSHeader)->AlignedTotalHeaderSz / (*ppFSHeader)->PartitionBlockSize;
    if (numHeaderBlocks > EXPECTED_NUM_HEADER_BLOCKS)
    {
        combinedHeaderBlocks.NumBlocks = numHeaderBlocks;
        combinedHeaderBlocks.BlockSize = (*ppFSHeader)->PartitionBlockSize;
        combinedHeaderBlocks.pBlockData = NULL;

        status = ReadBlocksFromFileSystem(&combinedHeaderBlocks, RawFSBlockIo);

        if (EFI_ERROR(status))
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders Error propagated from ReadBlocksFromFileSystem. status code: 0x%x.\n", status));
            goto Exit;
        }
    }
    *ppCombinedHeaders = combinedHeaderBlocks.pBlockData;
    *ppFSHeader = (PFS_HEADER)*ppCombinedHeaders;
    *ppDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)*ppCombinedHeaders + 1);

    if ((*ppDirHeader == 0) || (*ppFSHeader == 0))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders One of the file system headers is null. Returning EFI_VOLUME_CORRUPTED.\n"));
        status = EFI_VOLUME_CORRUPTED;
        goto Exit;
    }

    *ppFileEntries = (PFILE_ENTRY)(*ppCombinedHeaders + (*ppDirHeader)->FileEntriesStartOffset);

    if (*ppFileEntries == 0)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:RetrieveFileSystemHeaders One of the file system headers is null. Returning EFI_VOLUME_CORRUPTED.\n"));
        status = EFI_VOLUME_CORRUPTED;
        goto Exit;
    }


Exit:
    return status;
}

EFI_STATUS
AddFileRawFileSystem(
    IN UINT8 *pInputFileBuffer,
    IN UINTN InputFileBufferSize,
    IN CHAR16 *pFSFileName,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    PFS_HEADER pFSHeader = 0;
    PDIRECTORY_HEADER pDirHeader = 0;
    UINT8* pCombinedFSHeaders = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    FILE_UPDATE_INFO newFileInfo;
    FS_BLOCK fileWriteBlocks;
    FS_BLOCK headerWriteBlocks;
    EFI_STATUS status = EFI_SUCCESS;
    UINTN alignedInputFileSz = 0;

    ZeroMem(&newFileInfo, sizeof(FILE_UPDATE_INFO));
    ZeroMem(&fileWriteBlocks, sizeof(FS_BLOCK));
    ZeroMem(&headerWriteBlocks, sizeof(FS_BLOCK));

    // verify arguments are valid
    if ((pInputFileBuffer == NULL) || (pFSFileName == NULL) || (InputFileBufferSize == 0) || (RawFSBlockIo == NULL))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFileRawFileSystem One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    status = RetrieveFileSystemHeaders(&pCombinedFSHeaders, &pFSHeader, &pDirHeader, &pFileEntries, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        if (status == EFI_VOLUME_CORRUPTED)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFileRawFileSystem Signature and/or PartitionBlockSize is corrupted. Status = 0x%x.\n", status));
        }
        else
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFileRawFileSystem Could not retrieve file system headers. Status = 0x%x\n", status));
        }

        goto Exit;
    }

    if (!pCombinedFSHeaders || !pFSHeader || !pDirHeader)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFileRawFileSystem Retrieved File System Header is NULL!\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    if (GetNumFilesInFS(pCombinedFSHeaders, FALSE) >= pDirHeader->MaxFiles)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFileRawFileSystem No more files can be added to the filesystem.\n"));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    newFileInfo.pFSFilePath = pFSFileName;

    ALIGN_TO_BLOCK(InputFileBufferSize, pFSHeader->FSBlockSize, alignedInputFileSz);
    newFileInfo.pInputFileBuffer = pInputFileBuffer;
    newFileInfo.AllocationSize = alignedInputFileSz;
    newFileInfo.ActualSize = InputFileBufferSize;

    if (!newFileInfo.pInputFileBuffer)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFile Retrieved input file buffer is NULL!\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    status = PrepareAddFileWriteBlocks(pCombinedFSHeaders, &newFileInfo, &fileWriteBlocks, &headerWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFile PrepareWriteBuffers failed, status:0x%x.\n", status));
        goto Exit;
    }

    // Flush the file into file system

    status = FlushFSBuffer(&fileWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFile FlushFSBuffer failed, status:0x%x.\n", status));
        goto Exit;
    }

    // Flush headers into file system

    status = FlushFSBuffer(&headerWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AddFile FlushFSBuffer failed, status:0x%x.\n", status));
        goto Exit;
    }

Exit:
    if (pCombinedFSHeaders)
    {
        pCombinedFSHeaders = NULL;
    }

    if (newFileInfo.pInputFileBuffer)
    {
        newFileInfo.pInputFileBuffer = NULL;
    }

    if (fileWriteBlocks.pBlockData)
    {
        fileWriteBlocks.pBlockData = NULL;
    }

    return status;
}

EFI_STATUS
FlushFSBuffer(
    IN PFS_BLOCK pWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINT8 *pFileSystem = NULL;
    UINT8 *pFSHeader = NULL;
    UINTN fileSystemSize = 0;
    UINTN alignedSz = 0;

    // verify arguments are valid
    if ((pWriteBlocks == NULL) || (RawFSBlockIo == NULL))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FlushFSBuffer One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    /*
    ## HACK ##
    Read the entire buffer, overwrite specific blocks, then flush entire file system. This is because
    SetFilePointerEx seems to fail for some reason that needs to be looked into
    */
    // Get FS size
    ALIGN_TO_BLOCK(sizeof(FS_HEADER), RawFSBlockIo->Media->BlockSize, alignedSz);
    pFSHeader = AllocateZeroPool(alignedSz);
    if (pFSHeader == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }
    status = RawFSBlockIo->ReadBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
        (EFI_LBA)0,
        alignedSz,
        pFSHeader);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to read block data from partition.\n"));
        goto Exit;
    }

    status = RawFSBlockIo->FlushBlocks(RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to flush blocks.\n"));
        goto Exit;
    }
    fileSystemSize = ((PFS_HEADER)pFSHeader)->FileSystemSize;
    if (fileSystemSize == 0)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to open file because size is 0!\n"));
        goto Exit;
    }

    // Read FS blocks using aligned fileSystemSize
    ALIGN_TO_BLOCK(fileSystemSize, RawFSBlockIo->Media->BlockSize, alignedSz);
    pFileSystem = AllocateZeroPool(alignedSz);
    if (pFileSystem == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }
    status = RawFSBlockIo->ReadBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
        (EFI_LBA)0,
        alignedSz,
        pFileSystem);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to read block data from partition.\n"));
        goto Exit;
    }

    status = RawFSBlockIo->FlushBlocks(RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to flush blocks.\n"));
        goto Exit;
    }

    // Overwrite specific blocks in file system
    CopyMem(
        (pFileSystem + ((pWriteBlocks->BlockSize * pWriteBlocks->StartBlock))),
        pWriteBlocks->pBlockData,
        (pWriteBlocks->BlockSize * pWriteBlocks->NumBlocks)
        );

    // Write block data
    status = RawFSBlockIo->WriteBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
        (EFI_LBA)0,
        fileSystemSize,
        pFileSystem);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\nFailed to write block data to partition.\n"));
        goto Exit;
    }

    status = RawFSBlockIo->FlushBlocks(RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\nFailed to flush blocks.\n"));
        goto Exit;
    }

Exit:
    if (pFileSystem)
    {
        gBS->FreePool(pFileSystem);
        pFileSystem = NULL;
    }

    return status;
}

EFI_STATUS
ReadBlocksFromFileSystem(
    IN OUT PFS_BLOCK pFSBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINTN fileSystemSize = 0;
    UINTN alignedSz = 0;
    UINT8 *pFSHeader = NULL;

    // verify arguments are valid
    if (!pFSBlocks || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    // Get FS size
    ALIGN_TO_BLOCK(sizeof(FS_HEADER), RawFSBlockIo->Media->BlockSize, alignedSz);
    pFSHeader = AllocateZeroPool(alignedSz);
    if (pFSHeader == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }
    status = RawFSBlockIo->ReadBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
        (EFI_LBA)0,
        alignedSz,
        pFSHeader);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to read block data from partition.\n"));
        goto Exit;
    }

    status = RawFSBlockIo->FlushBlocks(RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to flush blocks.\n"));
        goto Exit;
    }
    fileSystemSize = ((PFS_HEADER)pFSHeader)->FileSystemSize;
    
    if (fileSystemSize == 0)
    {
        DEBUG((EFI_D_WARN, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to open file because size is 0!\n"));
        goto Exit;
    }

    // Read FS blocks using aligned fileSystemSize
	ALIGN_TO_BLOCK(pFSBlocks->NumBlocks * pFSBlocks->BlockSize, RawFSBlockIo->Media->BlockSize, alignedSz);
	pFSBlocks->pBlockData = (VOID*)AllocateZeroPool(alignedSz);
	if (pFSBlocks->pBlockData == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    status = RawFSBlockIo->ReadBlocks(RawFSBlockIo,
        RawFSBlockIo->Media->MediaId,
		(EFI_LBA)pFSBlocks->StartBlock,
        alignedSz,
		pFSBlocks->pBlockData);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReadBlocksFromFileSystem Failed to read block data from partition.\n"));
        goto Exit;
    }


Exit:

    return status;

}

VOID
SplitFilePath(
    IN CHAR16 *destPath,
    IN OUT CHAR16 *destFolderName,
    IN OUT UINT32 *pDestFolderNameLen,
    IN OUT CHAR16 *destFileName,
    IN OUT UINT32 *pDestFileNameLen)
{
    EFI_STATUS status = EFI_UNSUPPORTED;
    UINT32 destPathLen = (UINT32)StrLen(destPath);
    UINT16 slashIdx = 0;

    // verify arguments are valid
    if (!destPath || !destFolderName || !destFileName || !pDestFileNameLen || !pDestFolderNameLen)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:SplitFilePath One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    *pDestFolderNameLen = 0;
    *pDestFileNameLen = 0;

    if (destPathLen < sizeof(CHAR16))
    {
        goto Exit;
    }

    // Fail fast if start or end is with slash
    if ((destPath[0] == L'\\') ||
        (destPath[destPathLen - 1] == L'\\'))
    {
        goto Exit;
    }

    for (UINT16 charIdx = 0; charIdx < destPathLen; charIdx++)
    {
        if (destPath[charIdx] == L'\\')
        {
            if (slashIdx != 0)
            {
                // Fail due to two or more slashes
                DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:SplitFilePath More than one set of backslashes found in file path! Will bail out.\n"));
                goto Exit;
            }

            slashIdx = charIdx;
        }
    }

    if ((destPathLen - slashIdx) >= MAX_FILE_NAME_LENGTH)
    {
        // Fail due to length overflow
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:SplitFilePath File name is too long! Length: %u.\n", destPathLen - slashIdx));
        goto Exit;
    }

    // If the slash index is 0 it means we did not find a slash at all. We already fail fast if the beginning of the
    // string is a slash from earlier. This is OK because it means there is no folder name
    if (slashIdx == 0)
    {
        *pDestFileNameLen = destPathLen;
        CopyMem(destFileName, &destPath[0], *pDestFileNameLen * sizeof(CHAR16));
        goto Exit;
    }

    if ((slashIdx + 1) >= MAX_FOLDER_NAME_LENGTH)
    {
        // Fail due to length overflow
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:SplitFilePath Folder name is too long! Length: %u.\n", slashIdx + 1));
        goto Exit;
    }

    *pDestFolderNameLen = slashIdx;
    *pDestFileNameLen = destPathLen - slashIdx - 1;

    CopyMem(destFolderName, destPath, *pDestFolderNameLen * sizeof(CHAR16));
    CopyMem(destFileName, &destPath[slashIdx + 1], *pDestFileNameLen * sizeof(CHAR16));

    status = EFI_SUCCESS;

Exit:
    return;
}

BOOLEAN
FileExistsInFileSystem(
    UINT8 *pFileSystem,
    CHAR16 *pDestFileName,
    UINT32 *pFileIdx)
{

    BOOLEAN matchFound = FALSE;
    CHAR16 folderName[MAX_FOLDER_NAME_LENGTH] = { 0 }, fileName[MAX_FILE_NAME_LENGTH] = { 0 };
    UINT32 folderNameLen = 0;
    UINT32 fileNameLen = 0;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;

    // verify arguments are valid
    if (!pFileSystem || !pDestFileName || !pFileIdx)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FileExistsInFileSystem One or more agruments is invalid.\n"));
        goto Exit;
    }

    SplitFilePath(pDestFileName, folderName, &folderNameLen, fileName, &fileNameLen);

    // It is fair to not have a folder name, so we need to check if both file and folder are 0
    if (fileNameLen == 0 && folderNameLen == 0)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FileExistsInFileSystem pDestFileName is empty!\n"));
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pFileSystem;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pFileSystem + 1);

    if (!pFSHeader || !pDirHeader)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FileExistsInFileSystem File System offsets don't exist!\n"));
        goto Exit;
    }

    pFileEntries = (PFILE_ENTRY)(pFileSystem + pDirHeader->FileEntriesStartOffset);

    if (!pFileEntries)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:FileExistsInFileSystem File System offsets don't exist!\n"));
        goto Exit;
    }

    for (UINT16 fileEntryIdx = 0; fileEntryIdx < pDirHeader->MaxFiles; fileEntryIdx++)
    {
        if (StrCmp(pFileEntries[fileEntryIdx].FolderName, folderName) == 0)
        {
            if (StrCmp(pFileEntries[fileEntryIdx].FileName, fileName) == 0)
            {
                matchFound = TRUE;
                *pFileIdx = fileEntryIdx;
                goto Exit;
            }
        }
    }
Exit:
    return matchFound;
}

EFI_STATUS
PrepareDeleteFileWriteBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pFileUpdateInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;

    // verify arguments are valid
    if (!pCombinedFSHeaders || !pFileUpdateInfo || !pFileWriteBlocks || !pHeaderWriteBlocks || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareDeleteFileWriteBlocks One or more input parameters is null!\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    // Prepare PFILE_UPDATE_INFO for clearing out everything except offset and allocation size
    pFileUpdateInfo->FileNameLen = MAX_FILE_NAME_LENGTH;
    ZeroMem(pFileUpdateInfo->FileName, MAX_FILE_NAME_LENGTH);
    pFileUpdateInfo->FolderNameLen = MAX_FOLDER_NAME_LENGTH;
    ZeroMem(pFileUpdateInfo->FolderName, MAX_FOLDER_NAME_LENGTH);
    pFileUpdateInfo->ActualSize = 0;
    pFileUpdateInfo->AllocationSize = pFileEntries[pFileUpdateInfo->FileEntryIdx].AllocatedSize;
    pFileUpdateInfo->FileOffsetInData = pFileEntries[pFileUpdateInfo->FileEntryIdx].Offset;
    pFileUpdateInfo->BlockOffset = (pDirHeader->FileDataStartOffset + pFileUpdateInfo->FileOffsetInData) % pFSHeader->PartitionBlockSize;
    pFileUpdateInfo->pInputFileBuffer = AllocateZeroPool(pFileUpdateInfo->AllocationSize);
    if (pFileUpdateInfo->pInputFileBuffer == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }
    ZeroMem(pFileUpdateInfo->pInputFileBuffer, pFileUpdateInfo->AllocationSize);

    status = PrepareFileSystemBlocks(pCombinedFSHeaders, pFileUpdateInfo, pFileWriteBlocks, pHeaderWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareDeleteFileWriteBlocks PrepareFileSystemBlocks failed with status 0x%x.\n", status));
        goto Exit;
    }

    ReclaimAdjacentFragmentedMemory(pCombinedFSHeaders, pFileUpdateInfo->FileEntryIdx);

Exit:
    return status;
}

VOID
ReclaimAdjacentFragmentedMemory(
    IN UINT8 *pCombinedFSHeaders,
    IN UINT32 fileEntryIdx
)
{
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    UINT32 fileEndOffset = 0, contiguousSpaceLeft = 0, numFragmentedUnits = 0;

    // verify arguments are valid
    if (!pCombinedFSHeaders)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReclaimAdjacentFragmentedMemory WARNING: File System Buffer is NULL!\n"));
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    contiguousSpaceLeft = pDirHeader->TotalFileDataSpace - GetUsedSpaceInFS(pCombinedFSHeaders, TRUE);
    numFragmentedUnits = GetNumFilesInFS(pCombinedFSHeaders, TRUE);

    for (UINT32 fileEntryIterOuter = 0; fileEntryIterOuter < pDirHeader->MaxFiles; fileEntryIterOuter++)
    {
        for (UINT32 fileEntryIterInner = 0; fileEntryIterInner < pDirHeader->MaxFiles; fileEntryIterInner++)
        {
            // Optimization, we don't need to proceed if we don't have fragmented units
            if (numFragmentedUnits == 0)
            {
                goto Exit;
            }

            // If a file entry is valid, it is not fragmented
            // If a file entry is invalid and has no allocation, it is by definition not a fragmented unit to analyze
            if ((pFileEntries[fileEntryIterInner].Valid) || (pFileEntries[fileEntryIterInner].AllocatedSize == 0))
            {
                continue;
            }

            /* Check if the file is at the edge of contiguous memory. This occurs when the amount of contiguous memory
            remaining is the same [headers + file entries + file data offset + file allocation size].
            */
            fileEndOffset = pDirHeader->TotalFileDataSpace - (pFileEntries[fileEntryIterInner].Offset + pFileEntries[fileEntryIterInner].AllocatedSize);

            if (fileEndOffset == contiguousSpaceLeft)
            {
                contiguousSpaceLeft += pFileEntries[fileEntryIterInner].AllocatedSize;
                ZeroMem(&pFileEntries[fileEntryIterInner], sizeof(FILE_ENTRY));
                numFragmentedUnits--;
            }
        }
    }

Exit:
    return;
}

EFI_STATUS
DeleteFileRawFileSystem(
    IN CHAR16 *pFSFilePath,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINT8 *pCombinedFSHeaders = NULL;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    FS_BLOCK fileWriteBlocks, headerWriteBlocks;
    FILE_UPDATE_INFO fileUpdateInfo;

    ZeroMem(&fileWriteBlocks, sizeof(FS_BLOCK));
    ZeroMem(&headerWriteBlocks, sizeof(FS_BLOCK));
    ZeroMem(&fileUpdateInfo, sizeof(FILE_UPDATE_INFO));

    // verify arguments are valid
    if ((pFSFilePath == NULL) || (RawFSBlockIo == NULL))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFileRawFileSystem One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    status = RetrieveFileSystemHeaders(&pCombinedFSHeaders, &pFSHeader, &pDirHeader, &pFileEntries, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        if (status == EFI_VOLUME_CORRUPTED)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFileRawFileSystem Signature and/or PartitionBlockSize is corrupted. Status = 0x%x.\n", status));
        }
        else
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFileRawFileSystem Could not retrieve file system headers. Status = 0x%x.\n", status));
        }

        goto Exit;
    }

    if (!pCombinedFSHeaders)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile Retrieved File System Buffer is NULL!\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);

    if (!pFSHeader || !pDirHeader)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile File System offsets don't exist!\n"));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    if (!pFileEntries)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile File System offsets don't exist!\n"));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    if (FileExistsInFileSystem(pCombinedFSHeaders, pFSFilePath, &fileUpdateInfo.FileEntryIdx) == FALSE)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile File does not exist in file system!\n"));
        status = EFI_NOT_FOUND;
        goto Exit;
    }

    status = PrepareDeleteFileWriteBlocks(pCombinedFSHeaders, &fileUpdateInfo, &fileWriteBlocks, &headerWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile PrepareWriteBuffers failed with error code 0x%x.\n", status));
        goto Exit;
    }

    // Flush the file into file system

    status = FlushFSBuffer(&fileWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile FlushFSBuffer failed with error code 0x%x.\n", status));
        goto Exit;
    }

    // Flush headers into file system

    status = FlushFSBuffer(&headerWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:DeleteFile FlushFSBuffer failed with error code 0x%x", status));
        goto Exit;
    }
Exit:
    return status;
}


BOOLEAN ReuseFragmentedFileEntry(
    IN UINT8 *pFileSystem,
    IN UINT32 requiredAllocSz,
    OUT UINT32 *pFileEntryIdx
)
{
    BOOLEAN reusableFragmentFound = FALSE;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    UINT32 candidateFileEntryIdx = 0, freeSpaceAfterReuse = UINT32_MAX;

    // verify arguments are valid
    if (!pFileSystem || !pFileEntryIdx)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReuseFragmentedFileEntry One or more agruments is invalid.\n"));
        reusableFragmentFound = FALSE;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pFileSystem;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pFileSystem + 1);

    if (!pFSHeader || !pDirHeader)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReuseFragmentedFileEntry File System offsets don't exist!\n"));
        reusableFragmentFound = FALSE;

        goto Exit;
    }

    pFileEntries = (PFILE_ENTRY)(pFileSystem + pDirHeader->FileEntriesStartOffset);

    if (!pFileEntries)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:ReuseFragmentedFileEntry File System offsets don't exist!\n"));
        reusableFragmentFound = FALSE;

        goto Exit;
    }


    for (UINT16 fileEntryIdx = 0; fileEntryIdx < pDirHeader->MaxFiles; fileEntryIdx++)
    {
        // Find a fragment
        if ((pFileEntries[fileEntryIdx].Valid == FALSE) &&
            (pFileEntries[fileEntryIdx].AllocatedSize > 0))
        {
            // Check if fragment has memory larger than what we need, if yes, mark it as a candidate ONLY if it will
            // produce lower free space than the last candidate. Yes, 0 is our ideal free space after reuse

            if ((pFileEntries[fileEntryIdx].AllocatedSize >= requiredAllocSz) &&
                ((pFileEntries[fileEntryIdx].AllocatedSize - requiredAllocSz) < freeSpaceAfterReuse))
            {
                reusableFragmentFound = TRUE;
                candidateFileEntryIdx = fileEntryIdx;
                freeSpaceAfterReuse = pFileEntries[fileEntryIdx].AllocatedSize - requiredAllocSz;
            }
        }
    }

    *pFileEntryIdx = candidateFileEntryIdx;

    /* We have a #define that specifies the amount % leeway in free space, we do not want too much free space or else
    it would be a waste of allocated memory */

    if (reusableFragmentFound)
    {
        if (freeSpaceAfterReuse > (UINT32)(FRAGMENT_REUSE_FREE_SPACE_MAX * pFileEntries[candidateFileEntryIdx].AllocatedSize))
        {
            reusableFragmentFound = FALSE;
            *pFileEntryIdx = UINT32_MAX;
        }
    }
Exit:
    return reusableFragmentFound;
}

EFI_STATUS
GetFirstFreeFileEntry(
    IN UINT8 *pFileSystem,
    OUT UINT32 *pFileEntryIdx
)
{
    EFI_STATUS status = EFI_UNSUPPORTED;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;

    // verify arguments are valid
    if (!pFileSystem || !pFileEntryIdx)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetFirstFreeFileEntry One or more agruments is invalid."));
        goto Exit;
    }

    // Get the FS Header
    pFSHeader = (PFS_HEADER)pFileSystem;
    // Get the Directory Header
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pFileSystem + 1);
    // Get the FileEntries Header
    pFileEntries = (PFILE_ENTRY)(pFileSystem + pDirHeader->FileEntriesStartOffset);

    for (UINT16 fileEntryIdx = 0; fileEntryIdx < pDirHeader->MaxFiles; fileEntryIdx++)
    {
        if ((pFileEntries[fileEntryIdx].Valid == 0) &&
            (pFileEntries[fileEntryIdx].AllocatedSize == 0) &&
            (pFileEntries[fileEntryIdx].UsedSize == 0))
        {
            status = EFI_SUCCESS;
            *pFileEntryIdx = fileEntryIdx;

            goto Exit;
        }
    }
Exit:
    return status;
}

EFI_STATUS
GetNewFilePlacementInformation(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pNewFileInfo,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    UINT8 *pReallocatedBuffer = NULL;
    UINT8 *pTempBufPtr = NULL;

    // verify arguments are valid
    if (!pCombinedFSHeaders || !pNewFileInfo || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation One or more agruments is invalid.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    if (!pFSHeader || !pDirHeader)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation File System offsets don't exist!\n"));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);
    if (!pFileEntries)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation File System offsets don't exist!\n"));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    if (FileExistsInFileSystem(pCombinedFSHeaders, pNewFileInfo->pFSFilePath, &pNewFileInfo->FileEntryIdx) == TRUE)
    {
        pNewFileInfo->FileOffsetInData = pFileEntries[pNewFileInfo->FileEntryIdx].Offset;

        if (pNewFileInfo->AllocationSize <= pFileEntries[pNewFileInfo->FileEntryIdx].AllocatedSize)
        {
            /* If the allocated size is more than the incoming file's size, we must expand the incoming buffer to
            the allocated size and populate with zeros. This will ensure we erase older file data correctly

            Here is an example: Let's say existent memory has used = 900 and allocated = 1024. Incoming file is
            used = 400, allocated = 512. We need to make our allocated from 512 to 1024 as we are reusing the memory
            chunk. Furthermore, we need to clear 900-400 = 500 bytes of memory from older file or else we will have
            older file data lingering around in memory.
            */
            if (pNewFileInfo->AllocationSize < pFileEntries[pNewFileInfo->FileEntryIdx].AllocatedSize)
            {
                pReallocatedBuffer = AllocateZeroPool(pFileEntries[pNewFileInfo->FileEntryIdx].AllocatedSize);
                if (pReallocatedBuffer == NULL)
                {
                    status = EFI_OUT_OF_RESOURCES;
                    goto Exit;
                }
                ZeroMem(pReallocatedBuffer, pFileEntries[pNewFileInfo->FileEntryIdx].AllocatedSize);
                CopyMem(pReallocatedBuffer, pNewFileInfo->pInputFileBuffer, pNewFileInfo->AllocationSize);

                // Switch the Input buffer pointer and update allocation size
                pTempBufPtr = pNewFileInfo->pInputFileBuffer;
                pNewFileInfo->pInputFileBuffer = pReallocatedBuffer;

                gBS->FreePool(pTempBufPtr);
                pTempBufPtr = NULL;
                pReallocatedBuffer = NULL;

                pNewFileInfo->AllocationSize = pFileEntries[pNewFileInfo->FileEntryIdx].AllocatedSize;
            }
            // We can use the same file space
            status = EFI_SUCCESS;
            goto Exit;
        }
        else
        {
			/* 
				Incoming file has bigger allocation than what we have, we need to fragment the old file and add the new file. We first check if we
				have more file fragments/blocks available for a new allocation, otherwise we must fail the delete or else we will delete the file
				and then fail a write, losing both the original file and failing to add the new file.
			*/

			if (GetNumFilesInFS(pCombinedFSHeaders, TRUE) >= pDirHeader->MaxFiles)
			{
				status = EFI_OUT_OF_RESOURCES;
				goto Exit;
			}  
			status = DeleteFileRawFileSystem(pNewFileInfo->pFSFilePath, RawFSBlockIo);

            if (EFI_ERROR(status))
            {
                DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation DeleteFileRawFileSystem from FSBuffer failed with error code 0x%x.\n", status));
                goto Exit;
            }

            // Read updated headers

            FS_BLOCK updatedFSHeaders = { 0, 0, 0 };
            updatedFSHeaders.StartBlock = 0;
            updatedFSHeaders.NumBlocks = pFSHeader->AlignedTotalHeaderSz / pFSHeader->PartitionBlockSize;
            updatedFSHeaders.BlockSize = pFSHeader->PartitionBlockSize;

            status = ReadBlocksFromFileSystem(&updatedFSHeaders, RawFSBlockIo);

            if (EFI_ERROR(status))
            {
                DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation ReadBlocksFromFileSystem for updated FS headers failed with error code 0x%x.\n", status));
                goto Exit;
            }

            if (updatedFSHeaders.pBlockData == NULL)
            {
                status = EFI_UNSUPPORTED;
                DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation ReadBlocksFromFileSystem for updated FS headers produced null pBlockData"));
                goto Exit;
            }

            // Overwrite existing buffers
            CopyMem(pCombinedFSHeaders, updatedFSHeaders.pBlockData, pFSHeader->AlignedTotalHeaderSz);

            // Destroy buffer that was just created
            updatedFSHeaders.pBlockData = NULL;

            // For the increased file we can either reuse fragmented chunks or allocate from contiguous, the rest
            // of the function performs these checks
        }
    }
	else if (GetNumFilesInFS(pCombinedFSHeaders, TRUE) >= pDirHeader->MaxFiles)
	{
		status = EFI_OUT_OF_RESOURCES;
		goto Exit;
	}

    // If there is no match, we either recycle a fragmented FILE_ENTRY or we allocate a fresh pool of contiguous memory
    if (ReuseFragmentedFileEntry(pCombinedFSHeaders, pNewFileInfo->AllocationSize, &pNewFileInfo->FileEntryIdx) == TRUE)
    {
        // We can reuse a fragmented file entry
        pNewFileInfo->FileOffsetInData = pFileEntries[pNewFileInfo->FileEntryIdx].Offset;
        goto Exit;
    }

    // Otherwise allocate from contiguous pool

    status = GetFirstFreeFileEntry(pCombinedFSHeaders, &pNewFileInfo->FileEntryIdx);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation FindNextEmptyFileEntry failed with status 0x%x.\n", status));
        goto Exit;
    }

    status = AllocateFromFreeContiguousMemory(pCombinedFSHeaders, pNewFileInfo->AllocationSize, &pNewFileInfo->FileOffsetInData);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetNewFilePlacementInformation GetFreeContiguousMemory failed with status 0x%x.\n", status));
        goto Exit;
    }

Exit:
    return status;
}

EFI_STATUS AllocateFromFreeContiguousMemory(
    IN UINT8 *pCombinedFSHeaders,
    IN UINT32 requiredSize,
    IN UINT32 *pFileOffsetInData
)
{
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    UINT32 totalUsedSpace = 0, fileOffsetInFS = 0, contiguousMemoryLeft = 0;

    EFI_STATUS status = EFI_SUCCESS;

    // verify arguments are valid
    if (!pCombinedFSHeaders || !pFileOffsetInData)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AllocateFromFreeContiguousMemory File System Pointer is NULL!\n"));
        goto Exit;
    }

    // Get the FS Header
    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    // Get the Directory Header
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    // Get the FileEntries Header
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    totalUsedSpace = GetUsedSpaceInFS(pCombinedFSHeaders, TRUE);

    // File Data starts at FS_HEADER + DIRECTORY_HEADER + FILE_ENTRY(s) 
    // Contiguous memory (whatever is left) starts at file data + Valid Files Used Space + Fragmented Space
    fileOffsetInFS = pDirHeader->FileDataStartOffset + totalUsedSpace;
    contiguousMemoryLeft = pFSHeader->FileSystemSize - fileOffsetInFS;

    if (contiguousMemoryLeft < requiredSize)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:AllocateFromFreeContiguousMemory File system does not have enough contiguous memory left.\n"));
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    *pFileOffsetInData = totalUsedSpace;
Exit:
    return status;
}

UINT32 GetUsedSpaceInFS(
    IN UINT8 *pCombinedFSHeaders,
    IN BOOLEAN includeFragmented
)
{
    PFILE_ENTRY pFileEntries = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFS_HEADER pFSHeader = NULL;

    UINT32 usedSpace = UINT32_MAX;

    // verify arguments are valid
    if (!pCombinedFSHeaders)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetUsedSpaceInFS File System Pointer is NULL!\n"));
        goto Exit;
    }

    // Get the FS Header
    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    // Get the Directory Header
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);

    if (!pFSHeader || !pDirHeader)
    {
        if (!pCombinedFSHeaders)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetUsedSpaceInFS FS Header, Dir Header or File Entry Header is NULL!\n"));
        }
        goto Exit;
    }

    // Get the FileEntries Header
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    if (!pFileEntries)
    {
        if (!pCombinedFSHeaders)
        {
            DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:GetUsedSpaceInFS FS Header, Dir Header or File Entry Header is NULL!\n"));
        }
        goto Exit;
    }

    usedSpace = 0;

    for (UINT16 fildEntryIdx = 0; fildEntryIdx < pDirHeader->MaxFiles; fildEntryIdx++)
    {
        if ((pFileEntries[fildEntryIdx].Valid == 0) &&
            (includeFragmented == FALSE))
        {
            continue;
        }

        usedSpace += pFileEntries[fildEntryIdx].AllocatedSize;
    }

Exit:
    return usedSpace;
}

EFI_STATUS
PrepareFileSystemBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pFileUpdateInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
)
{
    EFI_STATUS status = EFI_SUCCESS;
    PFS_HEADER pFSHeader = NULL;
    PDIRECTORY_HEADER pDirHeader = NULL;
    PFILE_ENTRY pFileEntries = NULL;
    UINT32 fileEndOffsetBlockAligned = 0;

    // verify arguments are valid
    if (!pCombinedFSHeaders || !pFileUpdateInfo || !pHeaderWriteBlocks || !pFileWriteBlocks || !RawFSBlockIo)
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareFileSystemBlocks One or more passed parameters is NULL.\n"));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    pFSHeader = (PFS_HEADER)pCombinedFSHeaders;
    pDirHeader = (PDIRECTORY_HEADER)((PFS_HEADER)pCombinedFSHeaders + 1);
    pFileEntries = (PFILE_ENTRY)(pCombinedFSHeaders + pDirHeader->FileEntriesStartOffset);

    pFileUpdateInfo->BlockOffset = (pDirHeader->FileDataStartOffset + pFileUpdateInfo->FileOffsetInData) % pFSHeader->PartitionBlockSize;

    // Get the file end offset from start of partition
    fileEndOffsetBlockAligned = (pDirHeader->FileDataStartOffset + pFileUpdateInfo->FileOffsetInData + pFileUpdateInfo->AllocationSize);
    // Align the end to block end as the file has to consume
    // the whole block as new file cannot start from middle
    // of a block.
    ALIGN_TO_BLOCK(fileEndOffsetBlockAligned, pFSHeader->PartitionBlockSize, fileEndOffsetBlockAligned);
    
    // Get the start block, number of blocks and access size in preparation to read blocks
    pFileWriteBlocks->StartBlock = (pDirHeader->FileDataStartOffset + pFileUpdateInfo->FileOffsetInData) / pFSHeader->PartitionBlockSize;
    pFileWriteBlocks->NumBlocks = fileEndOffsetBlockAligned / pFSHeader->PartitionBlockSize;
	
	// if we are right at the edge of a disk block, we are good to go, if the module is not 0, there is data in the next block we need to read/write
    if ((pDirHeader->FileDataStartOffset + pFileUpdateInfo->FileOffsetInData + pFileUpdateInfo->AllocationSize) % pFSHeader->PartitionBlockSize)
	{
		pFileWriteBlocks->NumBlocks++;
	}

    pFileWriteBlocks->NumBlocks -= pFileWriteBlocks->StartBlock;
    pFileWriteBlocks->BlockSize = pFSHeader->PartitionBlockSize;

    status = ReadBlocksFromFileSystem(pFileWriteBlocks, RawFSBlockIo);

    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileFunctions:PrepareFileSystemBlocks ReadBlocksFromFileSystem failed with status: 0x%x.\n", status));
        goto Exit;
    }

    // Update the read blocks with incoming file buffer

    CopyMem(
        (pFileWriteBlocks->pBlockData + pFileUpdateInfo->BlockOffset),
        pFileUpdateInfo->pInputFileBuffer,
        pFileUpdateInfo->AllocationSize
        );

    // Update FILE_ENTRY for the requisite file

    pFileEntries[pFileUpdateInfo->FileEntryIdx].AllocatedSize = pFileUpdateInfo->AllocationSize;
    pFileEntries[pFileUpdateInfo->FileEntryIdx].UsedSize = pFileUpdateInfo->ActualSize;
    pFileEntries[pFileUpdateInfo->FileEntryIdx].Offset = pFileUpdateInfo->FileOffsetInData;
    pFileEntries[pFileUpdateInfo->FileEntryIdx].Valid = pFileUpdateInfo->AddFile;
    CopyMem(pFileEntries[pFileUpdateInfo->FileEntryIdx].FolderName, &(pFileUpdateInfo->FolderName), pFileUpdateInfo->FolderNameLen * sizeof(CHAR16));
    CopyMem(pFileEntries[pFileUpdateInfo->FileEntryIdx].FileName, &(pFileUpdateInfo->FileName), pFileUpdateInfo->FileNameLen * sizeof(CHAR16));

    pHeaderWriteBlocks->BlockSize = pFSHeader->PartitionBlockSize;
    pHeaderWriteBlocks->StartBlock = 0;
    pHeaderWriteBlocks->NumBlocks = pFSHeader->AlignedTotalHeaderSz / pFSHeader->PartitionBlockSize;
    pHeaderWriteBlocks->pBlockData = pCombinedFSHeaders;
Exit:
    return status;
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
    IN EFI_GUID                    *RootDeviceTypeGuid,
    IN EFI_GUID                    *PartTypeGuid,
    IN CHAR16                      *PartitionName,
    IN EFI_GUID                    *Protocol
)
{
    EFI_STATUS                 Status = EFI_SUCCESS;
    EFI_HANDLE                *HandleBuffer = NULL;
    UINTN                      NumHandles = 0;
    UINTN                      HandleCounter = 0;
    EFI_GUID                  *TempPartTypeGuid = NULL;
    CHAR16                    *TempPartitionName = NULL;
    EFI_PARTITION_ENTRY       *TempPartEntry = NULL;
    EFI_DEVICE_PATH_PROTOCOL  *DevPathInst = NULL;
    EFI_GUID                            TempRootDeviceGuid;

    //allow PartitionName to be NULL and match only GUID
    if ((PartHandle == NULL) ||
        (RootDeviceTypeGuid == NULL) ||
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
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] LocateHandleBuffer failed. Returning: %r.\n",
            __FUNCTION__, Status));
        goto GetPartitionHandle;
    }

    for (HandleCounter = 0; HandleCounter < NumHandles; HandleCounter++)
    {
        Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
            &gEfiPartitionTypeGuid,
            (VOID**)&TempPartTypeGuid);
        if (EFI_ERROR(Status))
        {
            continue;
        }

        Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
            &gEfiDevicePathProtocolGuid,
            (VOID**)&DevPathInst);
        if (EFI_ERROR(Status))
        {
            continue;
        }

        Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
            &gEfiPartitionRecordGuid,
            (VOID **)&TempPartEntry);
        if (EFI_ERROR(Status))
        {
            continue;
        }

        TempPartitionName = TempPartEntry->PartitionName;
        TempRootDeviceGuid = (EFI_GUID)((VENDOR_DEVICE_PATH*)DevPathInst)->Guid;
        DevPathInst = (EFI_DEVICE_PATH_PROTOCOL*)(((UINT8*)DevPathInst) + DevPathInst->Length[0]);      

        if (DevPathInst->Type == TYPE_MEDIA_DEVICE_PATH)
        {
            if ((CompareGuid(&TempRootDeviceGuid, RootDeviceTypeGuid)) &&
                (CompareGuid(TempPartTypeGuid, PartTypeGuid)))
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
    if (HandleBuffer != NULL)
    {
        gBS->FreePool(HandleBuffer);
        HandleBuffer = NULL;
    }

    return Status;
};


// Protocol functions
EFI_STATUS
FileRead(
    IN  EFI_RAW_FILE_SYSTEM_PROTOCOL    *This,
    IN CHAR16 *pReadFileName,
    IN OUT UINT8 *pOutBuffer,
    IN OUT UINTN *FileSize,
    IN CHAR16 *PartitionName,
    IN EFI_GUID *PartitionGuid,
    IN EFI_GUID *RootDeviceTypeGuid
)
{
    EFI_HANDLE  PartHandle = NULL;
    EFI_STATUS   status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo = NULL;

    if (!pReadFileName || !FileSize || !PartitionGuid)
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. One or more agruments is null.\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    /* Get the handle of given Disk type */
    status = GetPartitionHandle(&PartHandle,
        RootDeviceTypeGuid,
        PartitionGuid,
        PartitionName,
        &gEfiBlockIoProtocolGuid);

    // Get Block IO protocol interface handle
    status = gBS->HandleProtocol(PartHandle,
        &gEfiBlockIoProtocolGuid,
        (VOID**)&RawFSBlockIo);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. Returning: %r.\n", __FUNCTION__, status));
    }


    return ReadFileRawFileSystem(pReadFileName, FileSize, pOutBuffer, RawFSBlockIo);
}

EFI_STATUS
FileWrite(
    IN  EFI_RAW_FILE_SYSTEM_PROTOCOL    *This,
    IN UINT8                            *pInputFileBuffer,
    IN UINTN                             InputFileBufferSize,
    IN CHAR16                           *pFSFileName,
    IN CHAR16                           *PartitionName,
    IN EFI_GUID                         *PartitionGuid,
    IN EFI_GUID                         *RootDeviceTypeGuid
)
{
    EFI_HANDLE  PartHandle = NULL;
    EFI_STATUS   status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo = NULL;

    if (!pInputFileBuffer || !pFSFileName || !PartitionGuid)
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. One or more agruments is null.\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    /* Get the handle of given Disk type */
    status = GetPartitionHandle(&PartHandle,
        RootDeviceTypeGuid,
        PartitionGuid,
        PartitionName,
        &gEfiBlockIoProtocolGuid);

    // Get Block IO protocol interface handle
    status = gBS->HandleProtocol(PartHandle,
        &gEfiBlockIoProtocolGuid,
        (VOID**)&RawFSBlockIo);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. Returning: %r.\n", __FUNCTION__, status));
    }

    return AddFileRawFileSystem(pInputFileBuffer, InputFileBufferSize, pFSFileName, RawFSBlockIo);
}


EFI_STATUS
FileDelete(
    IN  EFI_RAW_FILE_SYSTEM_PROTOCOL    *This,
    IN CHAR16                           *pFSFilePath,
    IN CHAR16                           *PartitionName,
    IN EFI_GUID                     *PartitionGuid,
    IN EFI_GUID                     *RootDeviceTypeGuid
)
{
    EFI_HANDLE  PartHandle = NULL;
    EFI_STATUS   status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo = NULL;

    if (!pFSFilePath || !PartitionGuid)
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. One or more agruments is null.\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    /* Get the handle of given Disk type */
    status = GetPartitionHandle(&PartHandle,
        RootDeviceTypeGuid,
        PartitionGuid,
        PartitionName,
        &gEfiBlockIoProtocolGuid);

    // Get Block IO protocol interface handle
    status = gBS->HandleProtocol(PartHandle,
        &gEfiBlockIoProtocolGuid,
        (VOID**)&RawFSBlockIo);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. Returning: %r.\n", __FUNCTION__, status));
    }

    return DeleteFileRawFileSystem(pFSFilePath, RawFSBlockIo);
}


EFI_STATUS
FormatFileSystem(
    IN EFI_RAW_FILE_SYSTEM_PROTOCOL     *This,
    IN UINTN fsBlockSize,
    IN UINTN numCombinedHeaderBlocks,
    IN CHAR16 *PartitionName,
    IN EFI_GUID                     *PartitionGuid,
    IN EFI_GUID                     *RootDeviceTypeGuid
)
{
    EFI_HANDLE  PartHandle = NULL;
    EFI_STATUS   status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo = NULL;
    UINTN fileSystemSize = 0;

    if (!PartitionGuid)
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. One or more agruments is null.\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    /* Get the handle of given Disk type */
    status = GetPartitionHandle(&PartHandle,
        RootDeviceTypeGuid,
        PartitionGuid,
        PartitionName,
        &gEfiBlockIoProtocolGuid);

    // Get Block IO protocol interface handle
    status = gBS->HandleProtocol(PartHandle,
        &gEfiBlockIoProtocolGuid,
        (VOID**)&RawFSBlockIo);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. Returning: %r.\n", __FUNCTION__, status));
    }

    fileSystemSize = (RawFSBlockIo->Media->BlockSize / 1024) * ((RawFSBlockIo->Media->LastBlock) + 1);
    
    return FormatRawFileSystem(
        fsBlockSize,
        numCombinedHeaderBlocks,
        fileSystemSize,
        RawFSBlockIo
    );
}

BOOLEAN
VerifySignature(
    IN  EFI_RAW_FILE_SYSTEM_PROTOCOL    *This,
    IN CHAR16 *PartitionName,
    IN EFI_GUID                     *PartitionGuid,
    IN EFI_GUID                     *RootDeviceTypeGuid
)
{
    EFI_HANDLE PartHandle = NULL;
    EFI_STATUS status = EFI_SUCCESS;
    EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo = NULL;

    /* Get the handle of given Disk type */
    status = GetPartitionHandle(&PartHandle,
        RootDeviceTypeGuid,
        PartitionGuid,
        PartitionName,
        &gEfiBlockIoProtocolGuid);

    // Get Block IO protocol interface handle
    status = gBS->HandleProtocol(PartHandle,
        &gEfiBlockIoProtocolGuid,
        (VOID**)&RawFSBlockIo);
    if (EFI_ERROR(status))
    {
        DEBUG((EFI_D_WARN, "\n[%a] HandleProtocol(EfiBlockIoProtocolGuid) failed. Returning: %r.\n", __FUNCTION__, status));
    }

    return VerifyRawFileSystemSignature(RawFSBlockIo);
}

/**
EFI_RAW_FILE_SYSTEM Protocol implementation
*/
EFI_RAW_FILE_SYSTEM_PROTOCOL gEfiRawFileSystemProtocol =
{
    EFI_RAW_FILE_SYSTEM_PROTOCOL_REVISION,
    FileRead,
    FileWrite,
    FileDelete,
    FormatFileSystem,
    VerifySignature
};

/**

RawFileSystemEntryPoint()

Initializes Raw FS protocol

@param[in]  ImageHandle   The firmware allocated handle for the EFI image.
@param[in]  SystemTable   A pointer to the EFI System Table.

@return
EFI_SUCCESS               Raw FS protocol initialized successfully.
EFI_UNSUPPORTED           Raw FS protocol not supported.
EFI_XXXXXX                Failure occured. (Refer to UEFI spec for details)

**/

EFI_STATUS
RawFileSystemEntryPoint(
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS   status = EFI_SUCCESS;

    // Install protocol
    status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
        &gEfiRawFileSystemProtocolGuid,
        (void *)&gEfiRawFileSystemProtocol,
        NULL);

    if (EFI_ERROR(status)) {
        DEBUG((EFI_D_ERROR, "\n[RawFileSystemDxe]RawFileSystemEntryPoint: Failed to Install EfiRawFileSystem protocol.\n"));
    }

    return status;
}
