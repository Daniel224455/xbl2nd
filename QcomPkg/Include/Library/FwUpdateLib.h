/** @file FwUpdateLib.h

  Header for externally-available functions of the FwUpdateLib

  Copyright (c) 2011-2012,2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
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

#define UPDATE_ATTEMPT_COUNT_MAX 3

typedef enum
{
  QCOM_FW_UPDATE_MODE_QCFW,
  QCOM_FW_UPDATE_MODE_CAPSULE,
  QCOM_FW_UPDATE_MODE_NOUPDATE,
  QCOM_FW_UPDATE_MODE_MAX = QCOM_FW_UPDATE_MODE_NOUPDATE
}QCOM_FW_UPDATE_MODE;

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
 * The user Entry Point for FirmwareUpdate.
 *
 * @param Image              -Image to update
 * @param Image              -Size of the image to update
 * @param MsftAttemptStatus  -Microsoft last attempt status 
 * @param QcResultFlag       -QCFirmwareUpdate result flag
 *
 * @retval EFI_SUCCESS       The entry point is executed successfully.
 * @retval other             Some error occurs when executing this entry point.
 *
 **/
EFI_STATUS
EFIAPI
RunFwUpdateMain (
  IN CONST VOID                *Image,
  IN UINTN                      ImageSize,
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
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
  TODO

**/

EFI_STATUS
RecordFwUpdateStatus(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *ent,
  IN QCFW_UPDATE_FLAG_VALUE      QcResultFlag
  );

EFI_STATUS
FwUpdateClearFlagOrCapsule(
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

#endif
