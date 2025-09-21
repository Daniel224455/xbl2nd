/** @file FwPayloadEntry.h

  Payload entry structures and functions firmware update solution.

  Copyright (c) 2012, 2013, 2016, 2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2017/04/22     gparam      Changes to generalize FindMetaData function and remove
                            the hard dependency on global variable PayLoadHeader +
                            Changed the update status from internal type to
                            standard ones defined in EDK headers 
 2017/03/28      ssumit     Added payload buffer and size to support SetImage function
 2017/02/22      pkn        Added changes required to support new update type - FWCLASS_GUID
 2017/02/13      gpavithr   Added macro XBL_PARTITION_NAME
 2016/08/29      ltg        Replace FWENTRY_EMMC_PARTITION_TYPE with FWENTRY_DISK_PARTITION_TYPE
 2013/05/06      mic        Unsupport V1/V2 payload
 2012/09/21      mic        Initial version

=============================================================================*/

#ifndef __QCOM_FW_PAYLOADENTRY_H_
#define __QCOM_FW_PAYLOADENTRY_H_

#include <Library/FwCommonLib.h>
#include "FwEsrt.h"
#define QPAYLOAD_METADATA_SIGNATURE1  0x2E1946FB
#define QPAYLOAD_METADATA_SIGNATURE2  0x7F744D57

#define FILE_PATH_MAX_SIZE            255
#define DPP_NAME_MAX_SIZE             255
#define XBL_PARTITION_NAME            L"XBL"


typedef enum
{
  FWENTRY_OPERATION_TYPE_IGNORE   = 0x00000000,
  FWENTRY_OPERATION_TYPE_UPDATE   = 0x00000001,
  FWENTRY_OPERATION_TYPE_DELETE   = 0x00000002,
  FWENTRY_OPERATION_TYPE_MAX      = 0x00000003

}FWENTRY_OPERATION_TYPE;

typedef enum
{
  FWENTRY_OPERATION_PATH_TYPE_SOURCE  = 0x00000000,
  FWENTRY_OPERATION_PATH_TYPE_DEST    = 0x00000001,
  FWENTRY_OPERATION_PATH_TYPE_BACKUP  = 0x00000002,
  FWENTRY_OPERATION_PATH_TYPE_MAX     = 0x00000003

}FWENTRY_OPERATION_PATH_TYPE;

typedef enum
{
  FWENTRY_SOURCE_TYPE_FV         = 0x00000000,
  FWENTRY_SOURCE_TYPE_MAX        = 0x00000001

} FWENTRY_SOURCE_TYPE;

typedef enum
{
  FWENTRY_UPDATE_TYPE_PARTITION       = 0x00000000,
  FWENTRY_UPDATE_TYPE_FAT_FILE        = 0x00000001,
  FWENTRY_UPDATE_TYPE_DPP_QCOM        = 0x00000002,
  FWENTRY_UPDATE_TYPE_DPP_OEM         = 0x00000003,
  FWENTRY_UPDATE_TYPE_OPM_PRIV_KEY    = 0x00000004,
  FWENTRY_UPDATE_TYPE_SSD             = 0x00000005,
  FWENTRY_UPDATE_TYPE_FWCLASS_GUID    = 0x00000006,
  FWENTRY_UPDATE_TYPE_MAX             = 0x00000007

} FWENTRY_UPDATE_TYPE;

typedef enum
{
  FWENTRY_BACKUP_TYPE_PARTITION       = 0x00000000,
  FWENTRY_BACKUP_TYPE_FAT_FILE        = 0x00000001,
  FWENTRY_BACKUP_TYPE_MAX             = 0x00000002

} FWENTRY_BACKUP_TYPE;

/* One of PartitionName or PartitionTypeGUID must be specified, or both. If more than one partition of a given
   type is found, and name is not specified, then the first one found is used. */

#pragma pack(push)
#pragma pack(1)
typedef struct _FWENTRY_DEVICE_PATH
{
  FWENTRY_DISK_PARTITION_TYPE   DiskPartitionType;                      /* FWENTRY_DISK_PARTITION_TYPE: User, Boot1, Boot2, GPP */
  CHAR16                        PartitionName[PARTITION_NAME_MAX_CHAR16_SIZE]; /* Optional: Name string of destination partition in GPT */
  EFI_GUID                      PartitionTypeGUID;                      /* Optional: Destination Partition Type GUID to write file */
  CHAR16                        FileName[FILE_PATH_MAX_SIZE];           /* For FAT type or DPP item only: Unicode full path to the file including file name */

} FWENTRY_DEVICE_PATH;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct _FWENTRY_METADATA
{
  EFI_GUID               FileGuid;       /* File GUID that corresponds with FVUpdate.xml file item */
  FWENTRY_OPERATION_TYPE Operation;      /* FWENTRY_OPERATION_TYPE: The type of operation (Ignore, Update, delete)*/
  FWENTRY_UPDATE_TYPE    UpdateType;     /* FWENTRY_UPDATE_TYPE: Type of update (Partition, FAT, DPP, OPM_Priv) */
  FWENTRY_BACKUP_TYPE    BackupType;     /* FWENTRY_BACKUP_TYPE: Type of backup (None, Partition, FAT) */
  FWENTRY_DEVICE_PATH    UpdatePath;     /* Path to file or partition being updated */
  FWENTRY_DEVICE_PATH    BackupPath;     /* Path to file or partition used for backup */

} FWENTRY_METADATA;
#pragma pack(pop)

typedef struct _QPAYLOAD_FW_ENTRY
{
   VOID                       *DecryptedFwEntryBuffer;                   /* Pointer to the buffer with the decrypted file */
   UINTN                      DecryptedFwEntrySize;                      /* Size of the DecryptedFileBuffer*/
   BOOLEAN                    IsEncrypted;                               /* Flag to track if this entry was encrypted in the payload */
   VOID                      *Handle[FWENTRY_OPERATION_PATH_TYPE_MAX];   /* source/dest/backup handle for this entry*/
   FWENTRY_METADATA           Metadata;                                  /* Metadata entry extracted from payload*/

} QPAYLOAD_FW_ENTRY;

typedef struct _FV_BUFFER_HANDLE
{
   VOID                      *Buffer;                                    /* Pointer to the buffer with the FV content */
   UINTN                      BufferSize;                                /* Size of the FvBuffer */
} FV_BUFFER_HANDLE;

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
  );

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
  );

/**
 * Function to destruct   QPAYLOAD_FW_ENTRY
 *
 * @param Node - node for destruction

 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryDestructor(
  IN VOID *Data
  );

/**
 * Function to get  FwEntry linkedlist handle
 *
 * @param PartHandle - partition handle

 * @return : EFI_STATUS
 */
EFI_STATUS
FwGetFwEntryList(
    IN OUT FW_LINKED_LIST_TYPE **List
    );

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
  );

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
  );

/**
 *. This function should parse V3 payload
 * @param MetaData     - Metadata buffer
 * @param BufSize        - Metadata buffer size
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
  );


/**
 *   Function to validate a payload v3 fw entry
 *
 * @param Data           - Payload firmware entry metadata
 * @param bEntryGood - Pesult of validation
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwEntryValidation(
  IN FWENTRY_METADATA          *Data,
  OUT BOOLEAN                  *bEntryGood,
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
  );

/**
 * Function to decrypt a payload v3 fw entry
 *
 * @param FwEntry - payload fw entry

 * @return : EFI_STATUS
 */
EFI_STATUS
FwPayloadV3EntryCrypto(
    IN QPAYLOAD_FW_ENTRY *FwEntry
    );

#endif

