/** @file FwCapsule.c

  Capsule related functions

  Maintains a registry of the partitions needed for FwUpdate
    and provides lookup functions for these partitions and
    their appropriate read/write functions.

  Copyright (c) 2012, 2015,2017-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2019/03/21     vkasilya    Added FW_STATE_PREPARE_UPDATE to find FW Backup phase started
 2019/02/01     rajb        Added support for progress bar rendering, during capsule update
 2018/06/28     prema       Handle to update AttemptCount in corner cases.
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2017/04/18     gparam      Changes to centralize the recording of update status
                            in DPP for future use
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers + Removed
                            unused/redundant definitions/parameters + Modified existing
                            function prototype to match the FMP capsule implementation +
                            Moved the update code from FwQcFirmwareUpdatePhaseMain to
                            FwUpdateCapsulePhaseMain as the latest implementation is
                            capsule based though the same would be used by
                            QcFirmwareUpdate.exe as well + Added back the power check
                            and attempt count mechanism from legacy capsule flow
 2017/03/28     ssumit      Added payload buffer and size to support SetImage function
 2017/02/13     gpavithr    Changes to use ESRTDxe inplace of local functions
                            to directly create and/or operate on ESRT table
 2015/02/05     al          Changing TPL_NOTIFY to TPL_CALLBACK
 2012/10/05     mic         Added Data-Driven update support
 2012/08/10     mic         Changed debug message level for critical errors and
                            updated update attempt status error code
 2012/08/03     mic         Added maximum update attempts support
 2012/07/26     mic         Added configuration table pointer check before capsule
                            processing
 2012/07/25     rs          Added debug message for low battery level
 2012/07/02     Mic         Added debug message support for file node
 2012/06/12     rs          Added support for BreakChangeNumber
 2012/06/05     rs          Added support for displaying UXCapsule
 2012/05/15     Mic         Added robust capsule/Flag clean logic
 2012/05/07     rs          Added support for decryption of Boot Firmware
 2012/3/01      MiC         Initial version

=============================================================================*/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FwCommonLib.h>
#include <Library/FwUpdateLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Protocol/EFIEncryption.h>
#include <Pi/PiPeiCis.h>
#include <Library/SecBootSigLib.h>

#include "FwPowerCheck.h"
#include "FwPayloadEntry.h"
#include "FwResourceCheck.h"

extern QPAYLOAD_METADATA_HEADER_V3 PayLoadHeader;
extern FW_LINKED_LIST_TYPE         FwEntryList;

/*Below given order shall be as same as QCOM_FW_UPDATE_STATE_VARIABLES ENUM 
  and order shall not be changed*/
const CHAR16 *UpdateStateToStr[] = 
{
  L"FW_STATE_NOT_IN_PROGRESS",
  L"FW_STATE_PREPARE_UPDATE",
  L"FW_STATE_BACKUP_DONE",
  L"FW_STATE_UPDATE_SUCCESS",
  L"FW_STATE_UPDATE_FAIL",   
  L"FW_STATE_ROLLBACK_DONE"
};

/**
 * This function creates update attempt count NV
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdateCreateAttemptCountNV(
    VOID
)
{
  DEBUG((EFI_D_WARN,"Creating attempt count NV\n"));

  return SetAttemptCount(0);
}

/**
* Main function for update capsule 
*
* @param MsftAttemptStatus  -Microsoft last attempt status 
* @param QcResultFlag       -QCFirmwareUpdate result flag
*
* @retval EFI_SUCCESS       The entry point is executed successfully.
* @retval other             Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
FwUpdateCapsulePhaseMain (
  IN  VOID                     *Image,
  IN  UINTN                     ImageSize,
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
  )
{
  EFI_STATUS                      Status             = EFI_SUCCESS;
  EFI_STATUS                      Status1            = EFI_SUCCESS;
  UINT8                           AttemptCnt         = 0;
  QCOM_FW_UPDATE_STATE_VARIABLES  UpdateState        = FW_STATE_NOT_IN_PROGRESS;

  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  /*Resource availability checking*/
  Status = FwUpdateResourceCheckMain(MsftAttemptStatus,QcResultFlag);
  HANDLE_ERROR_LABEL(Status,FwUpdateResourceCheckMain,FwUpdateCapsulePhaseMainExit);

  Status = GetAttemptCount(&AttemptCnt);
  if (EFI_ERROR(Status)) 
  {
    DEBUG((EFI_D_WARN,"Failed to get attempt count from NV\n"));
    if (Status == EFI_NOT_FOUND)
    {
      Status = FwUpdateCreateAttemptCountNV();
      //We want update to continue if FwUpdateCreateAttemptCountNV failed.
      HANDLE_ERR_CONTINUE(Status,FwUpdateCreateAttemptCountNV);
    }
    AttemptCnt = 0;
  }

  if (AttemptCnt <= UPDATE_ATTEMPT_COUNT_MAX )
  {  
    AttemptCnt++;
  } 

  DEBUG((EFI_D_WARN,"    Current FW update attempt count is: %d\n", AttemptCnt));

  Status = GetUpdateState(&UpdateState);
  DEBUG((EFI_D_WARN,"    Current FW update state is: %s\n",UpdateStateToStr[UpdateState]));
  HANDLE_ERROR_LABEL(Status,FwUpdateCapsulePhaseMain:GetUpdateState,FwUpdateCapsulePhaseMainExit);

  if (UpdateState == FW_STATE_NOT_IN_PROGRESS)
  {
    Status = SetUpdateState(FW_STATE_PREPARE_UPDATE);
    HANDLE_ERROR_LABEL(Status,SetUpdateState(FW_STATE_PREPARE_UPDATE),FwUpdateCapsulePhaseMainExit);
  }

  /*Parsing payload metadata*/
  SysFwUpdateProgress(PROGRESS_CAPSULE_PROCESS_START);
  Status = FwPayloadParserMain(Image,ImageSize,MsftAttemptStatus,QcResultFlag);
  HANDLE_ERROR_LABEL(Status,FwPayloadParserMain,FwUpdateCapsulePhaseMainExit);
  SysFwUpdateProgress(PROGRESS_CAPSULE_PROCESS_END);

  /* If no FwEntry found, return success */
  if (FwEntryList.Count== 0)
  {
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
    *QcResultFlag = QCFW_UPDATE_SUCCESS;
    return EFI_SUCCESS;
  }

  Status = SetAttemptCount(AttemptCnt);
  HANDLE_ERR_CONTINUE(Status,SetAttemptCount);
 
  /*State machine function for firmware update*/
  Status = FwUpdatePhaseMain(MsftAttemptStatus,
                             QcResultFlag);

  HANDLE_ERROR_LABEL(Status, FwUpdatePhaseMain, FwUpdateCapsulePhaseMainExit);

  Status = EFI_SUCCESS;

FwUpdateCapsulePhaseMainExit:

  Status1 = SetAttemptCount(0);
  //We want to continue if SetAttemptCount failed.
  HANDLE_ERR_CONTINUE(Status1,SetAttemptCount(0));

  /* Clear the update state and attempt count in this error path as
     there is a chance to miss resetting this in FwUpdatePhaseMain
     during error */
  Status1 = SetUpdateState(FW_STATE_NOT_IN_PROGRESS);
  HANDLE_ERR_CONTINUE(Status1,SetUpdateState(FW_STATE_NOT_IN_PROGRESS));

  return Status;
}

