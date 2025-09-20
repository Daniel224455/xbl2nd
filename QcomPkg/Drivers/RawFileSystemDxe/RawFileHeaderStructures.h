/**
@file  RawFileFunctions.h
@brief Raw File System header related declarations.
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
#ifndef __RAW_FILE_STRUCTURES_H__
#define __RAW_FILE_STRUCTURES_H__


/*!
*      Raw File System Layout
*
*      +------------------------------+
*      |        RawFS Header          |
*      +------------------------------+
*      |        Directory Header      |
*      +------------------------------+
*      |        FILE ENTRY 0          |
*      +------------------------------+
*      |        ......                |
*      +------------------------------+
*      |        File Entry N - 1      |
*      +------------------------------+
*      |                              |
*      |                              |
*      |        File Data             |
*      |                              |
*      |                              |
*      +------------------------------+
*
*/

/*
*    File System Fragmentation
*
*   File system may be fragmented if a file entry space is cleared from the middle of the file. If the file is at the
*   end of the allocated memory then it can be reabsorbed into the contiguous memory.
*
*   A fragmented file will have the following FILE_ENTRY member variable characteristics:
*       Valid     = 0
*       Used      = 0
*       Allocated > 0
*
*   An unused FILE_ENTRY will have the following member variable characteristics
*       Valid     = 0
*       Used      = 0
*       Allocated = 0
*
*   The design allows for future defragmentation capabilities. The file system will be loaded in its entirety into memory
*   and all fragmented FILE_ENTRY allocations will be reclaimed as contiguous memory. All fragmented FILE_ENTRY will be
*   converted to unused FILE_ENTRY.
*
*   Defragmentation assumes the file system is small enough to be loaded onto memory.
*/

// Constants

#define MAX_FOLDER_NAME_LENGTH          48
#define MAX_FILE_NAME_LENGTH            48

/*!
* Data structure that describes the partition / file system header
*/
typedef struct _RAW_FS_HEADER
{
    UINT32  Signature;              /* Signature */
    UINT32  Revision;               /* Revision */
    UINT32  FSHeaderSz;             /* Total size of the header structure */
    UINT32  DirectoryOffset;        /* Offset of Directory Structure */
    UINT32  FSBlockSize;            /* Block Size of files and data within the file system */
    UINT32  PartitionBlockSize;     /* Block size for accessing file system, typically 4 KB for windows */
    UINT32  AlignedTotalHeaderSz;   /* Size of FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY combined, aligned to PartitionBlockSize*/
    UINT32  TotalHeaderSize;        /* Size of FS_HEADER, DIRECTORY_HEADER and FILE_ENTRY combined, without any alignment */
    UINT32  FileSystemSize;         /* Size of the entire file system */
    UINT8   Reserved2[32];          /* Reserved */
} FS_HEADER, *PFS_HEADER;

/*!
* Data structure that describes the Directory Header. There should be only one directory header
*/
typedef struct _RAW_DIRECTORY_HEADER_
{
    UINT32  Signature;                      /* Signature of the directory header, used for sanity check*/
    UINT32  DirHeaderSize;                  /* Directory Header size */
    UINT32  FileEntriesStartOffset;         /* Offset in file system from which the file entries begin to start. Files  entries store file metadata and not file data*/
    UINT32  TotalFileEntriesSize;           /* Total space used by the file entry structures */
    UINT32  MaxFiles;                       /* Max number of file entries allowed */
    UINT32  FileDataStartOffset;            /* Offset in partition from which the actual file data starts. Offset within File Descriptors are calculated using this as start base. */
    UINT32  TotalFileDataSpace;             /* Allocated space for file data in the partition, excluding all the headers and entries */
    UINT32  FragmentedSpace;                /* Data space that has been fragmented due to deletion of files */
    UINT32  ContiguousSpaceRemaining;       /* Amount of free contiguous space at the end of the file system */
    UINT8   Reserved[32];                   /* Reserved 32 bytes*/
} DIRECTORY_HEADER, *PDIRECTORY_HEADER;

/*!
* Data Structure that describes a file. This describes the size allocated size used validity and offset for the files.
* the offset is to be calculated using the the FileDataStartAddress from the directory header.

NOTE: FILE SYSTEM ONLY SUPPORTS ONE LEVEL OF HIERARCHY OF FILES AND FOLDERS. XX\YY.
*/
typedef struct _RAW_FILE_ENTRY_
{
    CHAR16  FileName[MAX_FILE_NAME_LENGTH + 1];           /* File Name, Unicode */
    CHAR16  FolderName[MAX_FOLDER_NAME_LENGTH + 1];       /* Parent Folder Name, Unicode */
    UINT32  AllocatedSize;                                /* Space allocated (aligned to FS_HEADER.FSBlockSize) */
    UINT32  UsedSize;                                     /* Space currently used, actual file size */
    BOOLEAN Valid;                                        /* Indicate if it is a valid entry */
    UINT32  Offset;                                       /* Offset of file from DIRECTORY_HEADER.FileDataStartOffset */
    UINT8   Reserved[32];                                 /* Reserved 32 bytes */
} FILE_ENTRY, *PFILE_ENTRY;
#endif
