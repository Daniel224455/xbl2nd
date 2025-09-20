/** @file FwGptHelpers.h
   
  Header for FwGptHelpers.h

  Copyright (c) 2011-2016, 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/07/11     prema       Changes to move GPT backup from file to NV
 2018/07/11     rajb        Added support to store active GPT data
 2018/05/21     gparam      Removed unused definitions
 2016/08/16     ltg         Replace "Emmc" with "DiskType"
 2015/04/15     mic         Added 64bit firmware update support
 2012/10/10     jd          Handle GPT switch based on firmware entry list  
 2012/10/08     mic         renamed gpt functions
 2011/10/12     hozefak     Initial version

=============================================================================*/

#ifndef __QCOM_FW_GPT_HELPERS_H__
#define __QCOM_FW_GPT_HELPERS_H__

#include "FwLinkedList.h"
#include <Library/FwUpdateLib/FwPayloadEntry.h>

#define ANYSIZE_ARRAY 1

//Maximum entries allowed for each partition of different memory types
#define UFS_BOOT_MAX_ENTRIES    0   // 0 LUN1, LUN2 uses ping pong
#define UFS_GPP_MAX_ENTRIES     40  // 32(UFS_LUN4) + 8 reserved for OEM expansion
#define SPINOR_BOOT_MAX_ENTRIES 36  // 28(SPINOR) + 8 reserved for OEM expansion
#define NVME_GPP_MAX_ENTRIES    16  // 8 (NVME) + 8 reserved for OEM expansion
#define EMMC_BOOT_MAX_ENTRIES   10  // 2 (XBL. XBL_CONFIG) + 8 reserved for OEM expansion
#define EMMC_GPP_MAX_ENTRIES    40  // 32(GPP) + 8 reserved for OEM expansion

typedef enum{
    ORIGINAL_TABLE,
    SWITCHED_TABLE
}GPT_TABLE_STATE;

typedef enum{
    BACKUP_GPT_TABLE,
    PRIMARY_GPT_TABLE
}GPT_TABLE_TYPE;

// Struct to store active GPT data
typedef struct _ACTIVE_GPT_ENTRY_SUB_DATA {
    EFI_GUID  PartitionTypeGUID;
    CHAR16    PartitionName[PARTITION_NAME_MAX_CHAR16_SIZE];
    UINT32    TableIndex;
} ACTIVE_GPT_ENTRY_SUB_DATA;

typedef struct _ACTIVE_GPT_SUB_DATA {
    UINT32                      Size;
    UINT32                      MaxEntries;
    UINT32                      UsedEntries;
    ACTIVE_GPT_ENTRY_SUB_DATA   ActiveGPTEntrySubData[ANYSIZE_ARRAY];
} ACTIVE_GPT_SUB_DATA;

typedef struct _COMPRESSED_DATA {
    UINT32                      Size;
    UINT32                      UncompressedSize;
    UINT8                       Data[ANYSIZE_ARRAY];
} COMPRESSED_DATA;

/**
 * Invalidate ALL Primary GPT Tables i.e USER BOOT and GPP
 * Partitions
 *
 * @param InvalidateBootGpt - Invalidate BOOT partition primary 
 *                          GPT.
 * @param InvalidateGppGpt - Invalidate GPP partition primary 
 *                         GPT.
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS InvalidateAllPrimaryGPT(
  IN BOOLEAN InvalidateBootGpt,
  IN BOOLEAN InvalidateGppGpt
  );

/**
 * Invalidate ALL Backup GPT Tables
 *
 * @param InvalidateBootGpt - Invalidate BOOT partition primary 
 *                          GPT.
 * @param InvalidateGppGpt - Invalidate GPP partition primary 
 *                         GPT.
 *
 * @return EFI_STATUS
 *
 */
EFI_STATUS InvalidateAllBackUpGPT(
   IN BOOLEAN InvalidateBootGpt,
   IN BOOLEAN InvalidateGppGpt
   );

/**
 * Validate the primary GPT 
 *
 * @param NONE
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS ValidatePrimaryGPT(VOID);


/**
 * Copy Boot Partition Table and GPP Partition Table to FAT16.bin in files BPT.TBL GPP.TBL
 *
 * @param BackupBootGpt : Indicate whether or not backup BOOT 
 *                      partition GPT
 * @param BackupGppGpt  : Indicate whether or not backup GPP 
 *                      partition GPT
 *
 * @return EFI _STATUS
 *
 */

EFI_STATUS BackupGPTTable(
   IN BOOLEAN BackupBootGpt,
   IN BOOLEAN BackupGppGpt
   );

/**
 * Modifies Boot Partition and GPP partition tables to point to the backup tables 
 * Also modifies and fills in the right CRC 
 * 
 * @param TableType
 * 
 * @param RestoreBootGpt 
 *  
 * @param RestoreGppGpt 
 * 
 * @return EFI_STATUS 
 */


EFI_STATUS RestoreAllGPTTablesFromDPP(
  IN GPT_TABLE_TYPE TableType,
  IN BOOLEAN RestoreBootGpt,
  IN BOOLEAN RestoreGppGpt
    );


/**
 * Modifies all gpt partition tables (BOOT partition and GPP)
 *
 * @param List - Firmware entry List 
 * @param ModifyBootGpt - Indicate whether or not modify BOOT 
 *                      GPT
 * @param ModifyGppGpt - Indicate whether or not modify GPP GPT 
 *
 * @return EFI_STATUS
 *
 */


EFI_STATUS ModifyAllBackupGptTablesForFailSafeUpdate(
    IN  FW_LINKED_LIST_TYPE *List,
    IN  BOOLEAN ModifyBootGpt,
    IN  BOOLEAN ModifyGppGpt
    );

/**
 * Compare GPT with the table in FAT partition and check if it the swapped table 
 * or it is the primary original table 
 *
 * @param TableType       :   Type of GPT Table 
 * @param DiskType_GUID   :   Vendor GUID whose GPT has to be 
 *                        compared 
 * @param gptTableState   :   returns if GPT table is in the
 *                            original or switched state
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS CmpGPTTable(
    IN GPT_TABLE_TYPE TableType,
	IN EFI_GUID DiskType_GUID,
    OUT GPT_TABLE_STATE* gptTableState
);


/**
 * Reads the partition table and returns Filesize and FileBuffer
 * with data from the GPT tables. In case FileBuffer is NULL
 * will return only the FileSize by reading the GPT header
 *
 * @param Memory_GUID     : Vendor GUID for which GPT tables
 *                          have to be read
 * @param FileBuffer      : Pointer to buffer where GPT should
 *                          be read. Iif NULL, Function will
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

EFI_STATUS ReadGPTTable(
    IN  EFI_GUID         Memory_GUID,
    OUT OPTIONAL VOID  **FileBuffer,
    OUT UINTN           *FileSize,
    OUT OPTIONAL UINT32 *SizeOfPartitionEntry,
    IN  GPT_TABLE_TYPE   TableType
);

/**
 * Invalidate the Primary GPT Table
 *
 * @param Memory_GUID
 *
 * @return EFI_STATUS
 *
 */


EFI_STATUS InvalidatePrimaryGPT(
        IN EFI_GUID Memory_GUID
        );

/**
 * Invalidate the Backup GPT Table
 *
 * @param Memory_GUID
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS InvalidateBackUpGPT(
        IN EFI_GUID Memory_GUID
        );

/**
 * This function goes through the firmware entry list and checks
 * if backup GPT of BOOT and GPP partitions needs to be 
 * switched for fail-safe update. 
 *  
 * @param List          :  Firmware entry list 
 * @param SwitchBootGpt :  Upon return, indicates if BOOT backup
 *                         GPT needs to be switched
 * @param SwitchGppGpt  :  Upon return, indicates if GPP backup 
 *                         GPT needs to be switched
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS GetAllSwitchGptRequestsFromFwEntryList(
  IN FW_LINKED_LIST_TYPE *List,
  OUT BOOLEAN *SwitchBootGpt,
  OUT BOOLEAN *SwitchGppGpt
  );

/**
 *  Function to allocate and init memory for all GPT backup table entries
 *
 * @return EFI_STATUS
 */
EFI_STATUS AllocateAndInitGPTBackupData(
    ACTIVE_GPT_SUB_DATA **BootGPTSubData,
    ACTIVE_GPT_SUB_DATA **GppGPTSubData
);

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
EFI_STATUS AllocateAndInitGPTSubData(
    IN UINT32                  MaxEntries,
    OUT ACTIVE_GPT_SUB_DATA  **GPTSubData
);

/**
 * This function will retrive active GPT data from GPT table
 * 
 * @param EFI_GUID                      :  Vendor GUID whose GPT has to be changed
 * @param UINT16                        :  File name to be stored
 * @param FWENTRY_DISK_PARTITION_TYPE   :  Type of disk partition to be filled
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS GetDiskPartAndGptNvType(
    IN EFI_GUID                                   Memory_GUID,
    OUT OPTIONAL QCOM_FW_UPDATE_NV_TYPE          *GPTNvTypePtr,
    OUT OPTIONAL FWENTRY_DISK_PARTITION_TYPE     *DiskPartTypePtr
);

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
EFI_STATUS AllocateAndInitCompressedData(
    IN UINT32                  UncompressedSize,
    IN OPTIONAL VOID          *InputData,
    IN UINT32                  InputDataSize,
    OUT COMPRESSED_DATA      **CompressedData
);

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
  );

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
  );

#endif
