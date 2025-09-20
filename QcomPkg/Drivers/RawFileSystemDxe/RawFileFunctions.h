/**
@file  RawFileFunctions.h
@brief Raw File System related declarations.
*/
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
05/31/18   prema   Fixed issues related to read and write files to raw file system
03/12/18   ltg     Initial revision

=============================================================================*/
#ifndef _RAW_FILE_FUNCTIONS_H_
#define _RAW_FILE_FUNCTIONS_H_

#include "RawFileHeaderStructures.h"

/*
Structures for auxiliary functions
*/

/*
@typedef
struct
@brief
Structure that carries details about a file update. File updates in the event of a file delete will carry mostly
empty information which will be used as source to write to the file headers and file data. In the event of a file
add this will be the carrier of FILE_ENTRY information and file data buffer.

*/
typedef struct _FILE_UPDATE_INFO_
{
    CHAR16  FolderName[MAX_FOLDER_NAME_LENGTH];                                     // Folder name of the file in the FS
    UINT32  FolderNameLen;                                                          // Length of the folder name
    CHAR16  FileName[MAX_FILE_NAME_LENGTH];                                         // File name of the file in the FS
    UINT32  FileNameLen;                                                            // Length of the file name
    CHAR16 *pFSFilePath;                                                            // File path of the file in the file system
    UINT32  AllocationSize;
    UINT32  ActualSize;
    UINT32  BlockOffset;
    UINT8  *pInputFileBuffer;                                                       // Pointer to buffer that contains input file
    UINT32  FileEntryIdx;                                                           // File entry index to modify
    UINT32  FileOffsetInData;
    BOOLEAN    AddFile;
}FILE_UPDATE_INFO, *PFILE_UPDATE_INFO;

typedef struct _FS_WRITE_BLOCKS_
{
    UINT32  StartBlock;                                                             // Block to start reading/writing from
    UINT32  NumBlocks;                                                              // Number of blocks of read/write to perform
    UINT32  BlockSize;                                                              // Block size for this read/write
    UINT8  *pBlockData;                                                             // Buffer that holds NumBocks * AccessSize amount of Filesystem data
}FS_BLOCK, *PFS_BLOCK;

/*
Primary File System Access and Manipulation Functions
*/

/*
@brief
Entry point of driver
Opens File System or re-formats if it is corrupted or not found.

@param
[In]  ImageHandle
[In]  SystemTable
@return
EFI_SUCCESS              --  driver exited successfully
Other standard EFI_STATUS codes in the event of an error
*/
EFI_STATUS
RawFileSystemEntryPoint(
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
);

/*
@brief
Function to create an empty Raw FS. This function will create default values for the headers (File system),
Directory, Files, etc) and populate them accordingly.

@param
[In] fsBlockSize                --  Block size of the file system. Headers and Entries will be aligned to this block size.
                                    It MUST be a factor of PartitionBlockSize.
[In] PartitionBlockSize         --  The block size used to access the file system. We do not expect the file system to be read
                                    as a whole. It must be a multiple of fsBlockSize.
[In] numCombinedHeaderBlocks    --  Number of access blocks that the combined (FS, DIRECTORY Headers + FILE_ENTRY) will span
[In] fsSizeKb                   --  Size of the file system in Kilobytes
[In] RawFSBlockIo               --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                  --  File System created successfully
Other standard EFI_STATUS codes

*/
EFI_STATUS
FormatRawFileSystem(
    IN UINT32 fsBlockSize,
    IN UINT32 numCombinedHeaderBlocks,
    IN UINT32 fsSizeKb,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that adds a file to the file system

Logic / Flow for adding a new file:

1. Open the file and get the file size
2. See if the file already exists in the file system
3. If file exists, it means we are updating a file
a. If it does, see if the incoming file will be able to use the same buffer allocation as older file
b. If we can reuse allocation, simply overwrite contents and update FILE_ENTRY
c. If we need a bigger allocation, we free the older allocation and allocate a new buffer from contiguous memory
3. If file does not exist, we can either reuse a fragmented piece of memory or allocate new memory from contiguous
4. Check if a fragmented memory block exists that is big enough to hold the file and only have at most 10% empty space
a. If we find such a block, we can reuse the allocation, without requiring contiguous memory to be lost
5. If we cannot find fragmented memory to recycle, we allocate a fresh amount of memory from contiguous pool

@param
[In] pInputFileBuffer      --      Path to the input file buffer
[In] InputFileBufferSize   --      Size of input file
[In] pFSFileName           --      Path ([Folder]\[File]) in the file system which the file will be saved as. [Folder] parameter is required.
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                  --  File added successfully
Other standard EFI_STATUS codes
*/
EFI_STATUS
AddFileRawFileSystem(
    IN UINT8 *pInputFileBuffer,
    IN UINTN InputFileBufferSize,
    IN CHAR16 *pFSFileName,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that deletes a file from the file system

@param
[In]     pFSFilePath      --      Path to the file to delete in the FS
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                  --  File deleted successfully
Other standard EFI_STATUS codes
*/

EFI_STATUS
DeleteFileRawFileSystem(
    IN CHAR16 *pFSFilePath,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that reads a file from the file system

@param
[In]     pReadFileName      --      Path to the file to read in the FS
[Out]    FileSize           --      Pointer to UINT to store file size
[In]     pOutBuffer        --       Pointer to buffer to store file contents
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                  --  File read successfully
Other standard EFI_STATUS codes
*/
EFI_STATUS
ReadFileRawFileSystem(
    IN CHAR16 *pReadFileName,
    IN OUT UINTN *FileSize,
    IN OUT UINT8 *pOutBuffer,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
Auxiliary functions
*/

/*
@brief
Function that reads the FS_HEADER to identify the size of the file system and retrieves
the entire file system into a buffer. The buffer is freed by caller function. The function assigns the pointers at
requisite offsets for FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY array.

@param
[In]  ppCombinedFSHeadersBuffer     --   Pointer to a pointer that will hold allocated buffer
[In]  ppFSHeader                    --   Pointer to a pointer to the start of the FS_HEADER information
[In]  ppDirHeader                   --   Pointer to a pointer to the start of the DIRECTORY_HEADER information
[In]  ppFileEntries                 --   Pointer to a pointer to the start of the FILE_ENTRY information
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                      --   File system was read into buffer
Standard EFI_STATUS codes for other errors
*/
EFI_STATUS
RetrieveFileSystemHeaders(
    OUT UINT8 **ppCombinedHeaders,
    OUT PFS_HEADER *ppFSHeader,
    OUT PDIRECTORY_HEADER *ppDirHeader,
    OUT PFILE_ENTRY *ppFileEntries,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function traverses through FILE_ENTRY structures and returns the total number of files in the file system. All files
have the Valid boolean set to TRUE in their FILE_ENTRY.

@param
[In]  pCombinedFSHeaders        --  Pointer to the file system
[In]  countFragmented           --  If TRUE, count num of fragmented files. If FALSE, count actual files.

@return
UINT32 number of files in file system. UINT32_MAX is returned if an error occurs within the function.
*/
UINT32 GetNumFilesInFS(
    IN UINT8 *pCombinedFSHeaders,
    IN BOOLEAN countFragmented
);

/*
@brief
Function that prepares the file system blocks that contain updated file information in the event of a file add. This will
include two FS_BLOCK structures, one for the file and one for the file system headers. One FlushFSBuffer is needed for
each of the FS_BLOCKs. This function does not perform the flush.

This is the "AddFile" analogue, and calls PerpareFileSystemBlocks, a function common to the "AddFile" and "DeleteFile"
variants.

@param
[In]     pCombinedFSHeaders    --  Pointer to buffer containing FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY array from file system
[In]     pNewfileInfo          --  Structure that carries FILE_ENTRY and file data for the incoming file
[Out]    pFileWriteBlocks      --  Pointer to FS_BLOCK structure that contains blocks pertinent to the file data
[Out]    pHeaderWriteBlocks    --  Pointer to FS_BLOCK structure that contains blocks pertinent to file headers
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS                 --  Blocks have been prepared and are ready to be flushed
Other standard EFI_STATUS codes
*/
EFI_STATUS
PrepareAddFileWriteBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pNewfileInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Common function to both PrepareAddFileWriteBlocks and PrepareDeleteFileWriteBlocks, this function uses the PFILE_UPDATE_INFO
data to read the required blocks from the file system update the contents at the right offsets. The function also updates
the CombinedFSHeaders and prepares a FS_BLOCK with the information, ready to be written
NOTE: this function reads the entire file system and then returns the specific blocks. This is a hack.
@param
[In]        pCombinedFSHeaders  -- Pointer to buffer containing FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY array from file system
[In]        pFileUpdateInfo     -- Structure that carries details about a file update
[InOut]     pFileWriteBlocks    -- Pointer to FS_BLOCK structure that contains blocks pertinent to the file data
[InOut]     pHeaderWriteBlocks  --  Pointer to PS_BLOCK structure which this function will allocate buffer memory to
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol
@return
EFI_SUCCESS                   --  FS_BLOCKS were prepared correctly
Other standard EFI_STATUS codes otherwise
*/
EFI_STATUS
PrepareFileSystemBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pFileUpdateInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that provides the index within the File Array that should be used for the next file, the file offset in the data
partition and pointer to the file start position. The outputs of this function will be used to add a file to the buffer.
This function identifies if we can reuse an existing fragmented memory piece, or whether allocation needs to be done from
contiguous memory.
@param
[In]  pCombinedFSHeaders    -- Pointer to buffer containing FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY array from file system
[In]  pNewfileInfo          --  Structure that carries FILE_ENTRY and file data for the incoming file
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol
@return
EFI_SUCCESS
Other standard EFI_STATUS codes in the event of an error
*/
EFI_STATUS
GetNewFilePlacementInformation(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pNewFileInfo,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that returns the first free file entry available for use.
@param
[In]  pFileSystem               --  Pointer to the file system
[Out] pFileEntryIdx             --  Pointer to UINT32 that stores the index within the FILE_ENTRY array to recycle
@return
EFI_SUCCESS
Other standard EFI_STATUS codes in the event of an error
*/
EFI_STATUS
GetFirstFreeFileEntry(
    IN UINT8 *pFileSystem,
    OUT UINT32 *pFileEntryIdx
);

/*
@brief
Function that determines if a fragmented memory can be recycled for an incoming file. Recall that a fragmented memory as
FILE_ENTRY valid = 0, allocatedSize > 0, usedSize = 0. A piece of fragmented memory will only be recycled if an incoming
file can occupy at least 90% of the allocated memory, otherwise the memory will not be used and contiguous memory will be
allocated instead. pFileEntryIdx return value should be ignored in the event FALSE is returned by function.
@param
[In]     pFileSystem            --  File system buffer in memory that is to be flushed
[In]     requiredAllocSz       --  Required allocation size for incoming file (incl. block alignment)
[Out]    pFileEntryIdx         --  Index within the FILE_ENTRY array which specifies the fragmented memory information
@return
TRUE                 --  File can use fragmented memory
FALSE in the event of an error
*/
BOOLEAN ReuseFragmentedFileEntry(
    IN UINT8 *pFileSystem,
    IN UINT32 requiredAllocSz,
    OUT UINT32 *pFileEntryIdx
);

/*
@brief
Function that determines if a file already exists in the file system.
@param
[In]     pFileSystem            --  File system buffer in memory
[In]     pDestFileName       --  Pointer to caller allocated memory that will hold the name of the destination file
[Out]    pFileIdx         --  Index within the FILE_ENTRY array which specifies the file
@return
TRUE --  File exists
FALSE -- otherwise
*/
BOOLEAN
FileExistsInFileSystem(
    UINT8 *pFileSystem,
    CHAR16 *pDestFileName,
    UINT32 *pFileIdx);

/*
@brief
Function that accepts a destination file path in the format [Folder]\[File] and breaks it up into folder and file names.
This is used by functions that need to work with FILE_ENTRY structures such as AddFileToFS and DeleteFileFromFS.
It is fair to not have a folder name.
@param
[In] destPath               --   Pointer to a pointer that will hold allocated buffer
[Out]  destFolderName       --   Pointer to caller allocated memory that will hold the name of the destination folder
[Out]  pDestFolderNameLen    --   Length of destFolderName in TCHAR, NOT bytes. Analogous to "string length"
[Out]  destFileName         --   Pointer to caller allocated memory that will hold the name of the destination file
[Out]  pDestFileNameLen      --   Length of destFileName in TCHAR, NOT bytes. Analogous to "string length"
@return
None
*/
VOID
SplitFilePath(
    IN CHAR16 *destPath,
    IN OUT CHAR16 *destFolderName,
    IN OUT UINT32 *pDestFolderNameLen,
    IN OUT CHAR16 *destFileName,
    IN OUT UINT32 *pDestFileNameLen);

/*
@brief
Function to read specific blocks from the file system. This function allocates the buffers required for the blocks, and
requires the caller to free the memory chunk.
NOTE: Due to SetFilePointeEx, this function reads the entire file system and then returns the specific blocks. This is a hack
@param
[InOut]     pFSBlocks         --  Pointer to FS_BLOCK structure which this function will allocate buffer memory to
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol
@return
EFI_SUCCESS                   --  Write blocks are prepared and ready to flush
Other standard EFI_STATUS codes otherwise
*/
EFI_STATUS
ReadBlocksFromFileSystem(
    IN OUT PFS_BLOCK pFSBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that prepares the file system blocks that contain updated file information in the event of a file delete. This
will include two FS_BLOCK structures, one for the file and one for the file system headers. One FlushFSBuffer is needed for
each of the FS_BLOCKs. This function does not perform the flush.
This is the "DeleteFile" analogue, and calls PerpareFileSystemBlocks, a function common to the "AddFile" and "DeleteFile"
variants.
@param
[In]     pCombinedFSHeaders    --  Pointer to buffer containing FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY array from file system
[In]     pFileUpdateInfo       --  Structure that carries FILE_ENTRY and file data for the file to delete
[Out]    pFileWriteBlocks      --  Pointer to FS_BLOCK structure that contains blocks pertinent to the file data
[Out]    pHeaderWriteBlocks    --  Pointer to FS_BLOCK structure that contains blocks pertinent to file headers
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol
@return
EFI_SUCCESS                 --  Write blocks are prepared and ready to flush
Other standard EFI_STATUS codes otherwise
*/
EFI_STATUS
PrepareDeleteFileWriteBlocks(
    IN UINT8 *pCombinedFSHeaders,
    IN PFILE_UPDATE_INFO pFileUpdateInfo,
    IN OUT PFS_BLOCK pFileWriteBlocks,
    IN OUT PFS_BLOCK pHeaderWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Function that determines if we have enough free contiguous memory to carve out for an incoming file, specified by
the required allocation size of the file. If there is enough memory, a pointer to the offset within the partition is
provided.
@param
[In]  pCombinedFSHeaders    --  Pointer to the file system
[In]  requiredSize          --  Required size to allocate for incoming file
[Out] pFileOffsetInData     --  Offset of the file in file data, stored in FILE_ENTRY
@return
EFI_SUCCESS              --  Allocation succeeded, and an offset has been returned
Other standard EFI_STATUS codes otherwise
*/
EFI_STATUS AllocateFromFreeContiguousMemory(
    IN UINT8 *pCombinedFSHeaders,
    IN UINT32 requiredSize,
    IN UINT32 *pFileOffsetInData
);

/*
@brief
Function traverses through FILE_ENTRY structures and returns the total number of bytes that have been allocated out
of the contiguous memory. If the BOOL includeFragmented is set to TRUE, fragmented memory is added as part of the total.
Fragmented Memory       : Valid = 0, AllocatedSize > 0, UsedSize = 0
Used Memory             : Valid = 1, AllocatedSize > 0, UsedSize >= 0;
The total amount of fragmented memory can be obtained by calling the function twice, first by setting the includeFragmented
boolean to TRUE and the second to FALSE. The difference yields the fragmentation.
The total amount of contiguous memory is [total amount of memory in the file data - GetUsedSpaceInFS (incl. fragmented)]
@param
[In]  pCombinedFSHeaders    --  Pointer to the file system
[In]  includeFragmented     --  Boolean to specify whether the total memory used should include fragmented memory or not
@return
UINT32 size of memory currently used up. UINT32_MAX is returned if an error occurs within the function.
*/
UINT32 GetUsedSpaceInFS(
    IN UINT8 *pCombinedFSHeaders,
    IN BOOLEAN includeFragmented
);

/*
@brief
Function reclaims adjacent free memory blocks into contiguous memory when a file is deleted. If the memory of a file
being deleted is adjacent to the contiguous memory space, we do not need to fragment it, but instead mark it as contiguous.
Adjacent fragmented memory blocks can also be reclaimed.
Because we do not support defragmentation yet, traversal of FILE_ENTRY array needs to start at the index of the file
that was just deleted backwards until the beginning of the array. FILE_ENTRY beyond the last deleted file cannot
contain memory that is beyond contiguous.
@param
[In]     pCombinedFSHeaders           --  File system buffer in memory that is to be flushed
[In]     fileEntryIdx          --  FILE_ENTRY[] index for the last file that was deleted
@return
none
*/
VOID
ReclaimAdjacentFragmentedMemory(
    IN UINT8 *pCombinedFSHeaders,
    IN UINT32 fileEntryIdx
);

/*
@brief
Function checks if the file system signature matches the raw file system signature
@param
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol
@return
TRUE    -- signatures match
FALSE   -- otherwise
*/
BOOLEAN
VerifyRawFileSystemSignature(
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

/*
@brief
Writes the requisite blocks back to the file system. Does not perform any freeing of caller provided memory.

Note: Due to issues with SetFilePointerEx, this function reads the entire file system, updates specific blocks at offsets
and then writes it back. This is a hack.

@param
[In]  pWriteBlocks          --  Pointer to FS_BLOCK structure that contains the block start, number of blocks and block buffers
[In] RawFSBlockIo          --  Pointer to handle for Block IO protocol

@return
EFI_SUCCESS              --  Blocks waere flushed successfully
Other standard EFI_STATUS codes in the event of an error
*/
EFI_STATUS
FlushFSBuffer(
    IN PFS_BLOCK pWriteBlocks,
    IN EFI_BLOCK_IO_PROTOCOL *RawFSBlockIo
);

#endif
