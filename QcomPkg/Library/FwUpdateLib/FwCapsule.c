/** @file FwCapsule.c

  Capsule related functions

  Maintains a registry of the partitions needed for FwUpdate
    and provides lookup functions for these partitions and
    their appropriate read/write functions.

  Copyright (c) 2012, 2015,2017 Qualcomm Technologies, Inc.  All Rights Reserved.
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

// TODO: Sumit: To Check the functionality be needed as legacy firmware update
// used this to limit the number of attempts to apply a system firmware.

/**
 * This function creates update attempt count file
 *
 * @return EFI_STATUS
 */
EFI_STATUS
FwUpdateCreateAttemptCountFile(
    VOID
)
{
  EFI_HANDLE PartHandle = NULL;
  EFI_STATUS Status = EFI_SUCCESS;
  VOID *Buffer = NULL;
  UINTN FileSize = 0;

  DEBUG((EFI_D_WARN,"Creating attempt count file in FS\n"));
  Status = FwUpdateGetHandleForPartition( QCOM_FW_UPDATE_PARTITION_GPP_FAT16, &PartHandle);
  if(EFI_ERROR(Status))
  {
    goto FwUpdateCreateAttemptCountFileExit;
  }

  FileSize = sizeof(UINT8);

  Status = gBS->AllocatePool (EfiBootServicesData , FileSize ,&Buffer );
  if(EFI_ERROR(Status))
  {
    goto FwUpdateCreateAttemptCountFileExit;
  }

  gBS->SetMem(Buffer, FileSize, 0);

  Status = FwUpdateWriteFileToFat(PartHandle, &Buffer, QCOM_FW_UPDATE_FILE_ATTEMPT_COUNT_FILE, &FileSize );
  if(EFI_ERROR(Status))
  {
    goto FwUpdateCreateAttemptCountFileExit;
  }

FwUpdateCreateAttemptCountFileExit:
  if (Buffer != NULL)
  {
    gBS->FreePool (Buffer);
    Buffer = NULL;
  }

  return Status;
}

/**
* Check if the FwVer from the payload is higher then the lowest supported version
*
* @param	bpass              the check result
*
* @return EFI_STATUS
**/

EFI_STATUS
FwUpdateCheckLowestSupportedVersion(
   OUT BOOLEAN  *bPass,
   OUT UINT32   *MsftAttemptStatus,
   OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
   )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_SYSTEM_RESOURCE_ENTRY   ent;

  if((bPass == NULL) ||
     (MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;
  *bPass = FALSE;

  Status = QcomGetEsrtEntryCache(&ent);
  if (EFI_ERROR(Status))
  {
    *QcResultFlag = QCFW_UPDATE_ERR_ESRT_GET_ENTRY;
    HANDLE_ERROR_LABEL(Status,QcomGetEsrtEntryCache,FwUpdateCheckLowestSupportedVersionExit);
  }

  if (PayLoadHeader.FirmwareVersion >= ent.LowestSupportedFwVersion)
  {
    *bPass = TRUE;
  }
  else
  {
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INCORRECT_VERSION;
    *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_LOWEST_SUPPORT_VERSION;

    *bPass = FALSE;
  }

FwUpdateCheckLowestSupportedVersionExit:

  return Status;

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
  BOOLEAN                         bVersionGood       = FALSE;
  BOOLEAN                         bPowerGood         = FALSE;
  QCOM_FW_UPDATE_STATE_VARIABLES  currentUpdateState = FW_STATE_NOT_IN_PROGRESS;
  UINT8                           AttemptCnt         = 0;

  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  //Get Block I/O and File Handles
  Status = UpdateHandleRegistry();
  if (EFI_ERROR(Status))
  {
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    *QcResultFlag = QCFW_UPDATE_ERR_HANDLE_REGISTRY;
    HANDLE_ERROR_LABEL(Status,UpdateHandleRegistry,FwUpdateCapsulePhaseMainExit);
  }

  /*Parsing payload metadata*/
  Status = FwPayloadParserMain(Image,ImageSize,MsftAttemptStatus,QcResultFlag);
  HANDLE_ERROR_LABEL(Status,FwPayloadParserMain,FwUpdateCapsulePhaseMainExit);

  /* If no FwEntry found, return success */
  if (FwEntryList.Count== 0)
  {
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
    *QcResultFlag = QCFW_UPDATE_SUCCESS;
    return EFI_SUCCESS;
  }

  /*Resource availability checking*/
  Status = FwUpdateResourceCheckMain(MsftAttemptStatus,QcResultFlag);
  HANDLE_ERROR_LABEL(Status,FwUpdateResourceCheckMain,FwUpdateCapsulePhaseMainExit);

  /*Check if the payload version is in the boundary of lowest supported version*/
  Status = FwUpdateCheckLowestSupportedVersion(&bVersionGood,MsftAttemptStatus,QcResultFlag);
  HANDLE_ERROR_LABEL(Status,FwUpdateCheckLowestSupportedVersion,FwUpdateCapsulePhaseMainExit);

  if (!bVersionGood)
  {
    Status = EFI_UNSUPPORTED;
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INCORRECT_VERSION;
    *QcResultFlag = QCFW_UPDATE_ERR_PAYLOAD_LOWEST_SUPPORT_VERSION;
    DEBUG_ERR_LABEL("\nLowest Supported Version failed\n",FwUpdateCapsulePhaseMainExit);
  }

  /* Check if the battery power level is sufficient for capsule update only
     if update is not already in-progress.  This allows an in-progress
     update to complete if the power level was on the boundary at the beginning
     of the update and a power cycle occurred which could prevent the
     update from completing. */
     
  Status = GetState(&currentUpdateState);
  if (EFI_ERROR(Status)) 
  {
    *QcResultFlag = QCFW_UPDATE_ERR_FLAG_GET_STAMACHINE_STATE;
    HANDLE_ERROR_LABEL(Status,GetState,FwUpdateCapsulePhaseMainExit);
  }
  
  if (currentUpdateState == FW_STATE_NOT_IN_PROGRESS)
  {
    /* LastAttemptStatus is set to an appropriate power event type if 
       bPowerGood is FALSE and the function returns success. */
    Status = FwUpdateCheckPowerLevel(&bPowerGood, MsftAttemptStatus,QcResultFlag);
    if (EFI_ERROR(Status))
    {
      /* Hard-code an error value of insufficient battery if failure retrieving power level. */
      *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_PWR_EVT_BATT;
      *QcResultFlag = QCFW_UPDATE_ERR_RESOUCE_POWER_INSUFFICEIENT_BATTERY; 
      HANDLE_ERROR_LABEL(Status,FwUpdateCheckPowerLevel,FwUpdateCapsulePhaseMainExit);
    }
    else if (bPowerGood == FALSE)
    { 
      DEBUG_ERR_LABEL("\nFwUpdate Failed: Insufficient battery.\n",FwUpdateCapsulePhaseMainExit);
    }
  }

  Status = GetAttemptCount(&AttemptCnt);
  if (EFI_ERROR(Status)) 
  {
    DEBUG((EFI_D_WARN,"Failed to get attempt count in FS\n"));
    if (Status == EFI_NOT_FOUND)
    {
      Status = FwUpdateCreateAttemptCountFile();
      //We want update to continue if FwUpdateCreateAttemptCountFile failed.
      HANDLE_ERR_CONTINUE(Status,FwUpdateCreateAttemptCountFile);
    }
    AttemptCnt = 0;
  }

  if (AttemptCnt <= UPDATE_ATTEMPT_COUNT_MAX )
  {  
    AttemptCnt++;
  } 

  DEBUG((EFI_D_WARN,"    Current update attempt count is %d\n", AttemptCnt));
  Status = UpdateAttemptCountInFS(AttemptCnt);
  HANDLE_ERR_CONTINUE(Status,UpdateAttemptCountInFS);
 
  /*State machine function for firmware update*/
  Status = FwUpdatePhaseMain(MsftAttemptStatus,
                             QcResultFlag);

  HANDLE_ERROR_LABEL(Status, FwUpdatePhaseMain, FwUpdateCapsulePhaseMainExit);

  return EFI_SUCCESS;

FwUpdateCapsulePhaseMainExit:

  return Status;
}

