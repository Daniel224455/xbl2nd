/** @file FwEntryIO_helpers.h
   
  Update/Backup/Rollback functions for a sigle firmware entry.

  Copyright (c) 2012, 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when         who         what, where, why
 ----------   ---         --------------------------------------------------
 2018/06/28   prema       Change to support multiple DPP entries and skip duplicate 
                          backup and delete operations in backup and rollback phase.
                          Changes to return unsupported for FAT and SSD Update types 
                          and to remove functions related to FAT and SSD processing
 2018/05/21   gparam      Changes related to cleanup of unused code
 2012/09/21   mic         Initial version

=============================================================================*/
#ifndef __QCOM_FW_ENTRYIO_HELPER_H__
#define __QCOM_FW_ENTRYIO_HELPER_H__

#include "FwPayloadEntry.h"

#define DEFAULT_COPY_LOOP_CNT 2

typedef
EFI_STATUS
(EFIAPI *FWENTRY_DELETE)(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry
  );

typedef
EFI_STATUS
(EFIAPI *FWENTRY_READ)(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN OUT VOID                          **Buffer,
    OUT UINTN                            *BufferSize
  );

typedef
EFI_STATUS
(EFIAPI *FWENTRY_WRITE)(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                             BufferSize
  );

/**
 * This function backup fw entry from destination path  to backup path.
 * If target entry doesn't exist in destination path, backup operation will 
 * be skipped
 *
 * @param FwEntry -  Firmware entry
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwEntryBackup(
    IN QPAYLOAD_FW_ENTRY *FwEntry
    );


/**
 * This function updates fw entry from payload path  to destination path.
 *
 * @param FwEntry -  Firmware entry 
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
FwEntryUpdate(
    IN QPAYLOAD_FW_ENTRY *FwEntry
    );

/**
 * This function rollback fw entry from backup path  to destination path.
 *
 * @param FwEntry -  Firmware entry
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
FwEntryRollback(
    IN QPAYLOAD_FW_ENTRY *FwEntry
    );


/**
 * This function delete FwEntry in the backup path 
 *
 * @param FwEntry -  Firmware entry
 * 
 * @return EFI_STATUS 
 */

EFI_STATUS
FwEntryDeleteBackup(
    IN QPAYLOAD_FW_ENTRY *FwEntry
    );

/**
 * Delete Partition entry
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDeletePartitionEntry(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry
    );


/**
 * Delete FAT entry
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDeleteFatEntry(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry
    );

/**
 * Delete Dpp entry
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @Param Type       - Dpp Type QCOM/OEM
 *
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDeleteDppEntry(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN CHAR16                            *Type
    );


/**
 * Delete QCOM Dpp entry
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDeleteQcomDppEntry(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry
    );


/**
 * Delete OEM Dpp entry
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDeleteOemDppEntry(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry
    );

/**
 * Reads Entry from an FV
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromFV(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize
    );

/**
 * Reads an entry from an unformatted partition
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size

 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromPartition(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize
    );

/**
 * Reads a entry from a FAT partition
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size

 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromFat(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize
    );

/**
 * Reads a DPP item from DPP Qcom or OEM path
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 * @param Type       - OEM or DPP 
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromDpp(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize,
    IN CHAR16                            *Type
    );

/**
 * Reads a DPP item from DPP Qcom path
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromDppQcom(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize
    );

/**
 * Reads a DPP item from DPP OEM path
 *  into a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateReadEntryFromDppOem(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              **Buffer,
    IN OUT UINTN                         *BufferSize
    );

/**
 * Writes a block file to an unformatted partition
 *  from a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateWriteEntryToPartition(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                             BufferSize
    );

/**
 * Writes a fat file to FAT partition from a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateWriteEntryToFat(
    IN FWENTRY_OPERATION_PATH_TYPE        PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                              BufferSize
    );

/**
 * Writes Dpp item to Dpp Qcom or OEM path from a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size 
 * @param Type       - QCOM or OEM 
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateWriteEntryToDpp(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                             BufferSize,
    IN CHAR16                            *Type
    );

/**
 * Writes Dpp item to Dpp Qcom path from a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateWriteEntryToDppQcom(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                             BufferSize
    );

/**
 * Writes Dpp item to Dpp Oem path from a buffer.
 * 
 * @param PathType   - Source/Dest/BackUp
 * @Param FwEntry    - FwEntry
 * @param Buffer     - Buffer to store the file
 * @param BufferSize - Buffer size
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateWriteEntryToDppOem(
    IN FWENTRY_OPERATION_PATH_TYPE       PathType,
    IN QPAYLOAD_FW_ENTRY                 *FwEntry,
    IN VOID                              *Buffer,
    IN UINTN                             BufferSize
    );


#endif
