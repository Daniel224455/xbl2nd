/** @file FwUpdateLib.h

  Header for externally-available functions of the FwUpdateLib

  Copyright (c) 2011-2012,2017-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2019/03/15     vkasilya    ResetHookEntryToDefault() declared.
 2019/03/13     vkasilya    Passing the progress fun ptr as a parameter to RunFwUpdateMain
 2019/02/11     rajb        Added test hook support to reset the device at differet phases
 2019/02/01     rajb        Changes to show progress within backup phase
 2019/02/01     rajb        Added support for progress bar rendering, during capsule update
 2018/05/21     gparam      Cleanup related to moving all file based state-machine
                            data to NV
 2017/04/18     gparam      Changes to centralize the recording of update status
                            in DPP for future use
 2017/04/18     gparam      Removed unused/redundant definitions/parameters
 2017/03/27     ssumit      Added payload buffer and size in argument to support SetImage function
 2012/08/03     mic         Added maximum update attempts support
 2012/05/17     Mic         Moved from OSAppPkg to QcomPkg
 2011/10/12     jthompso    Initial version

=============================================================================*/
#ifndef _FW_UPDATE_LIB_H_
#define _FW_UPDATE_LIB_H_

#include <Uefi.h>
#include <Library/FwUpdateLib/FwCapsule.h>
#include <Library/FwUpdateLib/FwEsrt.h>
#include <Protocol/FirmwareManagement.h>

#define UPDATE_ATTEMPT_COUNT_MAX 3

// Capsule update Progress values , start and end values edited here will reflect in the update progress bar
#define PROGRESS_CAPSULE_PROCESS_START          01
#define PROGRESS_CAPSULE_PROCESS_END            05
#define PROGRESS_CAPSULE_BACKUP_START           06
#define PROGRESS_CAPSULE_BACKUP_END             50
#define PROGRESS_CAPSULE_UPDATE_START           51
#define PROGRESS_CAPSULE_UPDATE_END             90
#define PROGRESS_CAPSULE_BACKUP_DELETE_START    91
#define PROGRESS_CAPSULE_BACKUP_DELETE_END      95
#define PROGRESS_CAPSULE_COMPLEATED             100

#define NO_RESET_IN_CURRENT_PHASE               0xFF

#define RESET_PHASE_DATA_NV                     L"ResetPhaseDataNV"
#define PROGRESS_BAR_CONFIG_NV                  L"EnableWideProgressBar"

typedef enum
{
  QCOM_FW_UPDATE_MODE_QCFW,
  QCOM_FW_UPDATE_MODE_CAPSULE,
  QCOM_FW_UPDATE_MODE_NOUPDATE,
  QCOM_FW_UPDATE_MODE_MAX = QCOM_FW_UPDATE_MODE_NOUPDATE
}QCOM_FW_UPDATE_MODE;

typedef enum
{
  QCOM_TESTHOOK_BACKUP_PHASE,
  QCOM_TESTHOOK_UPDATE_PHASE,
  QCOM_TESTHOOK_DELETE_PHASE,
  QCOM_TESTHOOK_ROLLBACK_PHASE,
  QCOM_TESTHOOK_MAX
}QCOM_TEST_HOOK_PHASE;

/*!
 * Data structure that describes the QPayload Metadata
 */
typedef struct _QPAYLOAD_METADATA_HEADER_V1
{
  UINT32  Signature1;               /* First signature: QPAYLOAD_METADATA_SIGNATURE1 */
  UINT32  Signature2;               /* Second signature: QPAYLOAD_METADATA_SIGNATURE2 */
  UINT32  Revision;                 /* Revision: 0x1 */
  UINT32  Size;                     /* Size in bytes of remaining data after the size field: 0x8 */
  UINT32  FirmwareVersion;          /* Version of the QPayload package */
  UINT32  LowestSupportedVersion;   /* Firmware cannot roll back to a version earlier than this */
} QPAYLOAD_METADATA_HEADER_V1;

/*!
 * Data structure that describes the QPayload Metadata with Breaking Change Number
 */
typedef struct _QPAYLOAD_METADATA_HEADER_V2
{
  UINT32  Signature1;               /* First signature: QPAYLOAD_METADATA_SIGNATURE1 */
  UINT32  Signature2;               /* Second signature: QPAYLOAD_METADATA_SIGNATURE2 */
  UINT32  Revision;                 /* Revision: 0x1 */
  UINT32  Size;                     /* Size in bytes of remaining data after the size field: 0x8 */
  UINT32  FirmwareVersion;          /* Version of the QPayload package */
  UINT32  LowestSupportedVersion;   /* Firmware cannot roll back to a version earlier than this */
  UINT32  BreakingChangeNumber;     /*Breaking Change Number to track if we need a double update*/
} QPAYLOAD_METADATA_HEADER_V2;

typedef struct _QPAYLOAD_METADATA_HEADER_V3
{
  UINT32  Signature1;               /* First signature: QPAYLOAD_METADATA_SIGNATURE1 */
  UINT32  Signature2;               /* Second signature: QPAYLOAD_METADATA_SIGNATURE2 */
  UINT32  Revision;                 /* Revision: QPAYLOAD_METADATA_REVISION */
  UINT32  Size;                     /* Size in bytes of remaining data after the size field */
  UINT32  FirmwareVersion;          /* Version of the QPayload package */
  UINT32  LowestSupportedVersion;   /* Firmware cannot roll back to a version earlier than this */
  UINT32  BreakingChangeNumber;     /* Breaking Change Number to track if we need a double update */
  UINT32  Reserved1;                /* Reserved for future use */
  UINT32  Reserved2;                /* Reserved for future use */
  UINT32  EntryCount;               /* Number of file entries to update */

} QPAYLOAD_METADATA_HEADER_V3;

/**
  This Function writes UEFI Log Buffer to LOGFS Partition

  @retval EFI_SUCCESS     File was written successfully
**/
EFI_STATUS
EFIAPI
WriteLogBufToPartition (
  VOID
  );

/**
  Do reset system.
**/
VOID
DoResetSystem (
  VOID
  );

/**
 * The user Entry Point for FirmwareUpdate.
 *
 * @param Image              -Image to update
 * @param Image              -Size of the image to update
 * @param MsftAttemptStatus  -Microsoft last attempt status 
 * @param QcResultFlag       -QCFirmwareUpdate result flag
 * @param Progress           -cb function ptr to report the Progress of FW update. 
 *
 * @retval EFI_SUCCESS       The entry point is executed successfully.
 * @retval other             Some error occurs when executing this entry point.
 *
 **/
EFI_STATUS
EFIAPI
RunFwUpdateMain (
  IN  CONST VOID                                   *Image,
  IN  UINTN                                         ImageSize,
  OUT UINT32                                       *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE                       *QcResultFlag,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress            OPTIONAL
  );

/**
  TODO

**/

EFI_STATUS
EFIAPI
FwUpdatePhaseMain (
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
  );

/**
 *  Update Sys Fw Ver Partition Entry.
 *
 *  @param FwVersion                   - Fw Version
 *  @param LowestSupportedFwVersion    - Lowest Supported Version
 *  @param LastAttemptStatus           - Last Attempt Status
 *  @param QcResultValue               - Result Value.
 *
 *  @return EFI_STATUS
 */

EFI_STATUS
RecordFwUpdateStatus(
  IN UINT32                   FwVersion,
  IN UINT32                   LowestSupportedFwVersion,
  IN UINT32                   LastAttemptStatus,
  IN QCFW_UPDATE_FLAG_VALUE   QcResultFlag
  );

/**
 * Function to update/clear QC result flag
 * 
 * @param QcResultFlag  -update result
 *
 * return EFI_STATUS
 */
EFI_STATUS
RecordQCResult(
  IN QCFW_UPDATE_FLAG_VALUE QcResultFlag
  );

/**
 * Gets the firmware version from the metadata file contained in the capsule payload
 *
 * @param Image - Payload to parse
 * @param ImageSize - Payload size
 * @param PayloadHeaderV3 - Payload header structure (optional)
 * @param MsftAttemptStatus - The last attempt status, which will be recorded in ESRT
 * @param QcResultFlag - QCFirmwareUpdate result flag
 *
 * @retval EFI_SUCCESS       The entry point is executed successfully.
 * @retval other             Some error occurs when executing this entry point.
 *
 **/
EFI_STATUS
EFIAPI
ExtractSystemFirmwareMetaDataHeader (
  IN VOID                          *Image,
  IN UINTN                          ImageSize,
  OUT QPAYLOAD_METADATA_HEADER_V3  *PayloadHeaderV3,
  OUT UINT32                       *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE       *QcResultFlag
  );


/**
  Callback funtion to report the process of the firmware updating.
  Wrap the caller's version in this so that progress from the device lib is within the expected range. 

  Authentication  : 01 to 05 %
  Capsule process : 06 to 10 %
  Capsule Backup  : 11 to 20 %
  Capsule Update  : 21 to 80 %
  Capsule Backup  : 81 to 90 %
  Capsule Commit  : 91 to 100 %

  @param[in]  Completion    A value between 1 and 100 indicating the current completion
  progress of the firmware update. Completion progress is
  reported as from 1 to 100 percent. A value of 0 is used by
  the driver to indicate that progress reporting is not supported.

  @retval EFI_SUCCESS       SetImage() continues to do the callback if supported.
  @retval other             SetImage() discontinues the callback and completes
  the update and returns.
**/
EFI_STATUS
EFIAPI
SysFwUpdateProgress (
  IN  UINTN                          Completion
);

/*
 *  Function will set the ResetEntryIndexInXXXXX phase variables to NO_RESET_IN_CURRENT_PHASE.
 */
VOID
ResetHookEntryToDefault(
  );
#endif
