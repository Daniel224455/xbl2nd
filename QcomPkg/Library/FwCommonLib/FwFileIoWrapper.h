/** @file FwFileIoWrapper.h

  I/O Wrapper functions for Firmware Volumes, Partitions, FAT
  and DPP

  Copyright (c) 2012,2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2017/03/28     ssumit      Added payload buffer and size to support SetImage function
 2012/11/14     mic         Added safe FAT write function
 2012/10/04     mic         Initial version

=============================================================================*/
#ifndef __QCOM_FW_FILEIO_WRAPPER_H__
#define __QCOM_FW_FILEIO_WRAPPER_H__

#include <Pi/PiFirmwareFile.h>
#include <Pi/PiFirmwareVolume.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/FwCommonLib.h>

/**
 * First write a file to give type of DPP by given buffer, buffer size and file 
 * name, then double check if it is written correctly.
 * 
 * @param Type       - Dpp type
 * @param FileName   - File name
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
FwUpdateDppWriteAndCheck(
    IN CHAR16     *Type,
    IN CHAR16     *FileName,
    IN VOID       *Buffer,
    IN OUT UINTN   BufferSize
    );

/**
 * Delete a DPP item by given Type,Name
 *
 * @param Type       - Dpp type
 * @param DppName    - Dpp name
 *
 * @return EFI_STATUS
 **/
EFI_STATUS
FwUpdateDppDelete(
    IN CHAR16                   *Type,
    IN CHAR16                   *DPPname

    );

/**
 * Write a buffer to DPP by given Type,Name,buffer and buffer size
 *
 * @param Type       - Dpp type
 * @param DppName    - Dpp name
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdateDppWrite(
    IN CHAR16                   *Type,
    IN CHAR16                   *DPPname,
    IN VOID                     *FileBuffer,
    IN UINTN                    BufferSize

    );


/**
 * Read from DPP into a buffer by a given Name.
 *
 * @param Type       - Dpp type
 * @param DppName    - Dpp name
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdateDppRead(
    IN CHAR16                   *Type,
    IN CHAR16                   *DPPname,
    OUT VOID                   **Buffer,
    OUT UINTN                   *BufferSize

    );

/**
 * Delete a partition by a given handle.
 *
 * @param PartHandle - Handle of partition
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdatePartitionDelete(
    IN EFI_HANDLE               PartHandle
    );

/**
 * Write to block by a given buffer, buffer size and handle.
 *
 * @param PartHandle - Handle of partition
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdatePartitionWrite(
    IN EFI_HANDLE               PartHandle,
    IN VOID                     *FileBuffer,
    IN UINTN                    BufferSize

    );

/**
 * Read from Block into a buffer by a given handle.
 *
 * @param PartHandle - Handle of partition
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdatePartitionRead(
    IN EFI_HANDLE               PartHandle,
    OUT VOID                    **FileBuffer,
    OUT UINTN                   *BufferSize

    );

/**
 * Read a file from FV into a buffer by a given guid and handle.
 *
 * @param PartHandle - Handle to FileSystem
 * @param NameGuid   - File Guid
 * @param FileBuffer - Buffer
 * @param BufferSize - Buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdateFvRead(
    IN EFI_HANDLE               PartHandle,
    IN EFI_GUID                 *NameGuid,
    OUT VOID                    **FileBuffer,
    OUT UINTN                   *BufferSize

  );

/**
 * Function to get FV handle
 *
 * @param FvHandle - FV handle

 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetFvHandle(
    IN VOID        *Image,
    IN UINTN        ImageSize,
    OUT EFI_HANDLE *FvHandle

  );
#endif
