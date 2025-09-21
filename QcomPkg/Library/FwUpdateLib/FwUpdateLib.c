/** @file FwUpdatePhaseLib.c

  Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/03/23     prema       Handled HANDLE_ERROR_LABEL with proper function
 2017/07/11     hhovale     Dealing error case in state machine.
 2017/05/08     gparam      Changes to centralize ESRT update to DPP to the end of
                            SetImage API + Minor tweaks to logging
 2017/04/22     gparam      New API to extract the metadata for use in very
                            early stage of udpate to retrieve the LastAttemptVersion
                            for logging early errors + Changes to centralize
                            the recording of update status in DPP for future use +
                            Added back the support to record QC specific failure
                            results in COMMIT flag file evenfor capsule update
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers + Removed
                            unused/redundant definitions/parameters +
                            Using FwUpdateCapsulePhaseMain in place of
                            FwQcFirmwareUpdatePhaseMain as the udpate code is
                            moved to this function
 2017/02/22     ssumit      Added payload buffer and size to support SetImage function
 2017/02/22     pkn         Added changes required to support new update type - FWCLASS_GUID
 2017/02/13     gpavithr    Changes to use ESRTDxe inplace of local functions
                            to directly create and/or operate on ESRT table
 2016/12/06     ssumit      Mounted EFI Partition with WD Partition label
                            to overcome earlier mount failures of EFI Partition for WD.
                            This is temporary fix and should be reverted
                            once we have FIX from UEFI team to mount EFI Partition.
 2016/08/29     ltg         Added UFS support
 2015/03/22     mic         Added Fwupdate support for MSM8994 LiQUID only
 2015/02/05     al          Changing TPL_NOTIFY to TPL_CALLBACK
 2014/12/08     mic         Removed FwGetOsType API
 2013/06/28     niting      Added WARN macro to prevent message in PRODMODE
 2013/01/26     mic         Added workaround for authentication protocol issue
 2012/11/30     mic         Added ESRT entry print
 2012/11/15     mic         Fixed a corner case issue on GPT table switching
 2012/10/10     jd          Handle GPT switch based on firmware entry list
 2012/10/05     mic         Added Data-Driven update support
 2012/08/10     mic         Changed debug message level for critical errors
 2012/08/03     mic         Added maximum update attempts support
 2012/07/26     mic         Added configuration table pointer check before capsule
                                     processing
 2012/07/17     mic         Added ToolSupportProtocol for OPM handshake and removed
                                     ToolSupportLib
 2012/07/13     mic         Added WinSecApp OPM handshake support
 2012/06/25     mic         Added support for OPM keys update
 2012/06/21     rs          Adding support for FPDT ACPI table
 2012/06/18     jd          rename OEM_UPDATE_EVENT_GUID
 2012/06/12     rs          Added support for BreakChangeNumber
 2012/06/05     rs          Added support for UXCapsule
 2012/06/04     rs          Replaced AsciiPrint with DEBUG macro
 2012/05/15     mic         Added robustness of the reset logic
 2012/05/07     rs          Added support for decryption of Boot Firmware
 2012/05/03     mic         Initial version

=============================================================================*/

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BootConfig.h>
#include <Pi/PiPeiCis.h>
#include <Library/QcomLib.h>
#include <Include/Library/FwCommonLib.h>
#include <Include/Library/FwUpdateLib.h>
#include <Include/api/storage/ufs_api.h>
#include <Include/Protocol/EFICardInfo.h>
#include <Library/SecBootSigLib.h>
#include <Protocol/EFIDpp.h>
#include <IndustryStandard/Acpi50.h>
#include <IndustryStandard/Bmp.h>
#include <Protocol/EFIEncryption.h>
#include <Protocol/EFIToolSupport.h>
#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIPlatformInfo.h>
#include "FwCapsule.h"
#include "FwEsrt.h"
#include "FwPayloadEntry.h"
#include "FwResourceCheck.h"
#include "FwEntryIO_helpers.h"


/*extern variable*/
extern QPAYLOAD_METADATA_HEADER_V3    PayLoadHeader;
extern BOOLEAN                        bDoubleUpdateNeeded;
extern EFI_GUID                       gEfiMemCardInfoProtocolGuid;


/*Static variable*/
static QCOM_FW_UPDATE_STATE_VARIABLES State ;

/*global variable*/
QCOM_FW_UPDATE_MODE                   FWupdateMode                  = QCOM_FW_UPDATE_MODE_NOUPDATE;

/**
 * Function to delete capsule or Qpayload.efi
 * 
 * @param QcResultFlag  -update result
 *
 * return EFI_STATUS
 */
EFI_STATUS
FwUpdateClearFlagOrCapsule (
  IN QCFW_UPDATE_FLAG_VALUE QcResultFlag
  )
{
  EFI_STATUS Status                = EFI_SUCCESS;
  UINT8      UpdateCommitFlagValue = QCFW_UPDATE_INIT;

  switch(FWupdateMode)
  {
    case QCOM_FW_UPDATE_MODE_QCFW:
    case QCOM_FW_UPDATE_MODE_CAPSULE:
    
      UpdateCommitFlagValue = (UINT8) QcResultFlag;
      
      Status = SetFlag(QCOM_FW_UPDATE_QC_RESULT_FLAG, (VOID*)&UpdateCommitFlagValue );
      HANDLE_ERROR_LABEL(Status,SetFlag,FwUpdateClearFlagOrCapsuleExit);

      break;

    case QCOM_FW_UPDATE_MODE_NOUPDATE:
    default:
      break;
  }

FwUpdateClearFlagOrCapsuleExit:

  FWupdateMode = QCOM_FW_UPDATE_MODE_NOUPDATE;
  return Status ;
}

/**
 *  Update ESRT  to both DPP and System Table and delete capsule.
 *
 * @param attemptatus - the update attempt status.
 *
 * @return EFI_STATUS
 */

EFI_STATUS
RecordFwUpdateStatus(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *ent,
  IN QCFW_UPDATE_FLAG_VALUE      QcResultValue
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = QcomUpdateEsrtEntry(ent, TRUE);
  //We want FupdateDone to continue if updating ESRT failed.
  HANDLE_ERR_CONTINUE(Status,QcomUpdateEsrtEntry);

  Status = UpdateAttemptCountInFS(0);
  //We want FupdateDone to continue if UpdateAttemptCountInFS failed.
  HANDLE_ERR_CONTINUE(Status,UpdateAttemptCountInFS);

  /*delete capsule or update commint flag and delete Qpayload
    Do not consider error from this function as fatal as this is just an
    additional info for QcFwUpdateTool.exe which is an internal tool */
  FwUpdateClearFlagOrCapsule(QcResultValue);

  return EFI_SUCCESS;
}

/**
 *  Function to backup FW entries in FwEntry linked list
 *
 * @return EFI_STATUS
 */

EFI_STATUS
StateFwEntryBackupStart (
   OUT UINT32                   *MsftAttemptStatus,
   OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
   )
{
  EFI_STATUS          Status        = EFI_SUCCESS;
  UINTN               NodeCount     = 0;
  FW_LIST_NODE_TYPE   *Node         = NULL;
  QPAYLOAD_FW_ENTRY   *pFwEntry      = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList  = NULL;
  UINTN               i             = 0;

  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,StateFwEntryBackupStartExit);

  Status = LinkedListGetHeadNode(FwEntryList,&Node);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,StateFwEntryBackupStartExit);

  if (Node == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,StateFwEntryBackupStartExit);

  // Backup BOOT and GPP/LUN4 GPT to FAT
  Status = CopyPartitionTabletoFat16(TRUE, TRUE);
  HANDLE_ERROR_LABEL(Status,CopyPartitionTabletoFat16,StateFwEntryBackupStartExit);

  for (i = 0; i < NodeCount; i++)
  {
    DEBUG((EFI_D_WARN,"    Backing up %d/%d FwEntry...\n",i+1,NodeCount));
    if (Node != NULL)
    {
      pFwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;
      if (pFwEntry != NULL)
      {
        /* LUN1 and LUN2 will not be backup */
        if ((pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN1) ||
            (pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN2))
        {
          Node = Node->Next;
          DEBUG((EFI_D_ERROR, "      Backup Success\n\n"));
          continue;
        }

        Status = FwEntryBackup(pFwEntry);
        if(EFI_ERROR(Status))
        {
          DEBUG((EFI_D_ERROR,"      Backup failed\n\n"));
        }
        else
        {
          DEBUG((EFI_D_ERROR,"      Backup Success\n\n"));
        }

        HANDLE_ERROR_LABEL(Status,FwEntryBackup,StateFwEntryBackupStartExit);
      }
      else
      {
        Status = EFI_INVALID_PARAMETER;
        *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_NULL_ENTRY;
        DEBUG_ERR_LABEL("ListNode->Data == NULL ",StateFwEntryBackupStartExit);
      }

      Node = Node->Next;
    }
    else
    {
      Status = EFI_INVALID_PARAMETER;
      *QcResultFlag = QCFW_UPDATE_ERR_LIST_NULL_NODE;
      DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryBackupStartExit);
    }
  }

  Status = UpdateStateinFS(FW_STATE_BACKUP_DONE);
  HANDLE_ERROR_LABEL(Status,UpdateStateinFS,StateFwEntryBackupStartExit);

  return EFI_SUCCESS;

StateFwEntryBackupStartExit:

  FwPrintFwEntry(pFwEntry,EFI_D_ERROR);

 return Status;
}

/**
 *  Function to update FW entries in FwEntry linked list
 *
 * @return EFI_STATUS
 */

EFI_STATUS
StateFwEntryUpdateStart(
   OUT UINT32                   *MsftAttemptStatus,
   OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
)
{
  GPT_TABLE_STATE      BOOT_GPT_STATE = ORIGINAL_TABLE;
  GPT_TABLE_STATE      GPP_GPT_STATE  = ORIGINAL_TABLE;
  EFI_STATUS           Status         = EFI_SUCCESS;
  UINTN                NodeCount      = 0;
  FW_LIST_NODE_TYPE   *Node           = NULL;
  QPAYLOAD_FW_ENTRY   *pFwEntry       = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList   = NULL;
  UINTN                i              = 0;
  BOOLEAN              SwitchBootGpt  = FALSE;
  BOOLEAN              SwitchGppGpt   = FALSE;
  EFI_GUID             BOOT_GUID;
  EFI_GUID             GPP_GUID;

  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPartitionGUIDs(&BOOT_GUID, &GPP_GUID);
  HANDLE_ERROR_LABEL(Status, GetPartitionGUIDs, StateFwEntryUpdateStartExit);

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,StateFwEntryUpdateStartExit);

  Status = LinkedListGetHeadNode(FwEntryList,&Node);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,StateFwEntryUpdateStartExit);

  if (Node == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
    *QcResultFlag = QCFW_UPDATE_ERR_LIST_NULL_NODE;
    DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryUpdateStartExit);
  }

  Status = LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,StateFwEntryUpdateStartExit);

  // Check if need to backup BOOT or GPP/LUN4 GPT
  Status = GetAllSwitchGptRequestsFromFwEntryList(FwEntryList, &SwitchBootGpt, &SwitchGppGpt);
  HANDLE_ERROR_LABEL(Status,GetAllSwitchGptRequestsFromFwEntryList,StateFwEntryUpdateStartExit);

  // We might have encountered a reset hence first check if we have the right
  // backup GPT table if not correct it and then invalidate it

  // Only do this is GPT switch is needed
  if (SwitchBootGpt)
  {
    // At this point, GPT should be backup to FAT16 already in previous backup phase.
    Status = CmpGPTTable(BACKUP_GPT_TABLE, BOOT_GUID, &BOOT_GPT_STATE);
    HANDLE_ERROR_LABEL(Status,CmpGPTTable,StateFwEntryUpdateStartExit);
  }

  if (SwitchGppGpt)
  {
    // At this point, GPT should be backup to FAT16 already in previous backup phase.
    Status = CmpGPTTable(BACKUP_GPT_TABLE, GPP_GUID, &GPP_GPT_STATE);
    HANDLE_ERROR_LABEL(Status,CmpGPTTable,StateFwEntryUpdateStartExit);
  }

    // boot GPT table for UFS system does not need to be modified
  if((BOOT_GPT_STATE == ORIGINAL_TABLE) && (SwitchBootGpt == TRUE) && (!boot_from_ufs()))
  {
    Status = ModifyAllBackupGptTablesForFailSafeUpdate(FwEntryList,
                                                       SwitchBootGpt,
                                                       FALSE);
    HANDLE_ERROR_LABEL(Status,ModifyAllBackupGptTablesForFailSafeUpdate,StateFwEntryUpdateStartExit);
  }

  if ((GPP_GPT_STATE == ORIGINAL_TABLE) && (SwitchGppGpt == TRUE))
  {
    Status = ModifyAllBackupGptTablesForFailSafeUpdate(FwEntryList,
                                                       FALSE,
                                                       SwitchGppGpt) ;
    HANDLE_ERROR_LABEL(Status,ModifyAllBackupGptTablesForFailSafeUpdate,StateFwEntryUpdateStartExit);
  }

  // Invalidate primary GPT if needed
  Status = InvalidateAllPrimaryGPT(SwitchBootGpt, SwitchGppGpt);
  HANDLE_ERROR_LABEL(Status,InvalidateAllPrimaryGPT,StateFwEntryUpdateStartExit);

  for (i = 0; i < NodeCount; i++)
  {
    DEBUG((EFI_D_WARN,"    Updating %d/%d FwEntry...\n",i+1,NodeCount));
    if (Node != NULL)
    {
      pFwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;
      if (pFwEntry != NULL)
      {
        Status = FwEntryUpdate(pFwEntry);
        if(EFI_ERROR(Status))
        {
          DEBUG((EFI_D_ERROR,"      Update failed\n\n"));
        }
        else
        {
          DEBUG((EFI_D_ERROR,"      Update Success\n\n"));
        }
        HANDLE_ERROR_LABEL(Status,FwEntryUpdate,StateFwEntryUpdateStartExit);
      }
      else
      {
        Status = EFI_INVALID_PARAMETER;
        *QcResultFlag = QCFW_UPDATE_ERR_FWENTRY_NULL_ENTRY;
        DEBUG_ERR_LABEL("ListNode->Data == NULL ",StateFwEntryUpdateStartExit);
      }
      Node = Node->Next;
    }
    else
    {
      Status = EFI_INVALID_PARAMETER;
      *QcResultFlag = QCFW_UPDATE_ERR_LIST_NULL_NODE;
      DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryUpdateStartExit);
    }
  }

  Status = UpdateStateinFS(FW_STATE_UPDATE_SUCCESS);
  HANDLE_ERROR_LABEL(Status,UpdateStateinFS,StateFwEntryUpdateStartExit);

  return EFI_SUCCESS;

StateFwEntryUpdateStartExit:

  FwPrintFwEntry(pFwEntry,EFI_D_ERROR);

  return Status;
}

/**
*  Function to set the LUN (LUN1 or LUN2) as bootable where the
*  update was applied. It will determine the current bootable LUN
*  (LUN1 or LUN2) and it will set bootable the other LUN
*
* @return EFI_STATUS
*/
EFI_STATUS
setBootableLUNPostUpdate(void)
{
  EFI_STATUS                 Status           = EFI_SUCCESS;
  EFI_MEM_CARDINFO_PROTOCOL *MemCardInfo      = NULL;
  UINT32                     currentBootLun   = 0;
  UINT32                     newBootLun       = 0;
  UINT32                     LunNumber1       = 1;
  UINT32                     LunNumber2       = 2;


  Status = GetBootLUN(&currentBootLun);
  HANDLE_ERROR_LABEL(Status, GetBootLUN, setBootableLUNPostUpdateExit);

  if (currentBootLun == LunNumber1)
  {
    newBootLun = LunNumber2;
  }
  else if (currentBootLun == LunNumber2)
  {
    newBootLun = LunNumber1;
  }
  else
  {
    Status = EFI_NOT_FOUND;
    DEBUG_ERR_LABEL("Could not determine Boot LUN", setBootableLUNPostUpdateExit);
  }

  Status = GetMemCardInfoHandle(&MemCardInfo);
  HANDLE_ERROR_LABEL(Status, GetMemCardInfoHandle, setBootableLUNPostUpdateExit);

  Status = MemCardInfo->SetBootLU(MemCardInfo, newBootLun);
  HANDLE_ERROR_LABEL(Status, MemCardInfo->GetBootLU, setBootableLUNPostUpdateExit);

setBootableLUNPostUpdateExit:
  return Status;
}

/**
 *  Function to update Flags/State in post update done state
 *
 * @return EFI_STATUS
 */
EFI_STATUS
StateFwEntryPostUpdateStart(
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
  )
{
  EFI_STATUS           Status         = EFI_SUCCESS;
  FW_LINKED_LIST_TYPE *FwEntryList    = NULL;
  UINTN                NodeCount      = 0;
  FW_LIST_NODE_TYPE   *Node           = NULL;
  BOOLEAN              RestoreBootGpt = FALSE;
  BOOLEAN              RestoreGppGpt  = FALSE;
  BOOLEAN              BootLunUpdated = FALSE;
  UINTN                i              = 0;
  QPAYLOAD_FW_ENTRY   *pFwEntry       = NULL;


  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }
  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,StateFwEntryPostUpdateStartExit);

  Status = GetAllSwitchGptRequestsFromFwEntryList(FwEntryList, &RestoreBootGpt, &RestoreGppGpt);
  HANDLE_ERROR_LABEL(Status,GetAllSwitchGptRequestsFromFwEntryList,StateFwEntryPostUpdateStartExit);

  // We could have come back to this state after a reboot, hence we need to verify
  // the state of the primary table and if is switched shows UEFI copied backup
  // table to primary

  Status = RestoreAllGPTTablesFromFat(PRIMARY_GPT_TABLE, RestoreBootGpt, RestoreGppGpt);
  HANDLE_ERROR_LABEL(Status,RestoreAllGPTTablesFromFat,StateFwEntryPostUpdateStartExit);

  Status = RestoreAllGPTTablesFromFat(BACKUP_GPT_TABLE, RestoreBootGpt, RestoreGppGpt);
  HANDLE_ERROR_LABEL(Status,RestoreAllGPTTablesFromFat,StateFwEntryPostUpdateStartExit);

  Status = LinkedListGetHeadNode(FwEntryList,&Node);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,StateFwEntryPostUpdateStartExit);

  if (Node == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
    *QcResultFlag = QCFW_UPDATE_ERR_LIST_NULL_NODE;
    DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryPostUpdateStartExit);
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,StateFwEntryPostUpdateStartExit);

  for (i = 0; i < NodeCount; i++)
  {
    DEBUG((EFI_D_WARN,"    Deleting %d/%d FwEntry in backup device path...\n",i+1,NodeCount));

    if (Node != NULL)
    {
      pFwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;
      if (pFwEntry != NULL)
      {
        /* Find out which LUN was updated */
        if ((pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN1)
         || (pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN2)
         || (pFwEntry->Metadata.BackupPath.DiskPartitionType == FWENTRY_UFS_LUN1)
         || (pFwEntry->Metadata.BackupPath.DiskPartitionType == FWENTRY_UFS_LUN2))
        {
          BootLunUpdated = TRUE;
        }

        /*Not deleting the FWCLASS_GUID bkp at this point. This will be instead
        deleted jus after the update is complete and while the status gets
        updted in ESRT provision file in DPP */
        if(pFwEntry->Metadata.UpdateType != FWENTRY_UPDATE_TYPE_FWCLASS_GUID)
        {
          Status = FwEntryDeleteBackup(pFwEntry);
          if(EFI_ERROR(Status))
          {
            FwPrintFwEntry(pFwEntry, EFI_D_WARN);
            DEBUG((EFI_D_ERROR,"      Delete failed...Continue\n\n"));
          }
          else
          {
            DEBUG((EFI_D_ERROR,"      Delete Success\n\n"));
          }
          HANDLE_ERR_CONTINUE(Status,FwEntryDeleteBackup);
        }
        else
        {
          DEBUG((EFI_D_ERROR,"      Delete of Fw GUID file postponed to end of update\n\n"));
        }
      }
      else
      {
        DEBUG((EFI_D_ERROR,"  Can not delete a NULL FwEntry\n\n"));
      }
      Node = Node->Next;
    }
    else
    {
      DEBUG((EFI_D_ERROR,"  Can not delete a NULL Node\n\n"));
      break;
    }
  }

  /* Identify and set updated LUN as bootable */
  if (boot_from_ufs() && BootLunUpdated)
  {
    Status = setBootableLUNPostUpdate();
    HANDLE_ERROR_LABEL(Status, setBootableLUNPostUpdate, StateFwEntryPostUpdateStartExit);
  }

  Status = UpdateStateinFS(FW_STATE_NOT_IN_PROGRESS);
  HANDLE_ERROR_LABEL(Status,UpdateStateinFS,StateFwEntryPostUpdateStartExit);

  //Write the new BreakingChangeNumber to FS
  Status = SetBreakingChangeNumber(PayLoadHeader.BreakingChangeNumber);

  if(Status != EFI_SUCCESS)
  {
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
    *QcResultFlag = QCFW_UPDATE_SUCCESS;
    HANDLE_ERR_CONTINUE(Status,SetBreakingChangeNumber);
  }
  else if(bDoubleUpdateNeeded)
  {
    //This is a Breaking Change Case. A double-update is needed. Do not clear the commit flag

    /*System might have up to 6 attempts after resetting the attempt count, but we still want to reset it since
    the new UEFI.mbn in the new payload may interpret the counter differently.*/
    Status = UpdateAttemptCountInFS(0);
    HANDLE_ERR_CONTINUE(Status,UpdateAttemptCountInFS);

    // System Reset is still required if UpdateAttemptCountInFs failed.
    //Force Reset here.
    DEBUG((EFI_D_WARN,"\n\nDouble update required. Resetting System...\n\n"));
    gRT->ResetSystem(EfiResetCold,
                     EFI_SUCCESS,
                     0,
                     NULL) ;
  }
  else
  {
    //This is NOT a Breaking Change Case. A double-update is NOT needed. Clear the commit flag.
    /*Clear flag*/
    *MsftAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
    *QcResultFlag = QCFW_UPDATE_SUCCESS;
  }

  return EFI_SUCCESS;

StateFwEntryPostUpdateStartExit:

    return Status;
}

/**
 *  Function to Rollback all FW entries
 *
 * @return EFI_STATUS
 */
EFI_STATUS
StateFwEntryRollbackAllStart(
  )
{
  EFI_STATUS          Status       = EFI_SUCCESS;
  UINTN               i            = 0;
  UINTN               NodeCount    = 0;
  FW_LIST_NODE_TYPE   *Node        = NULL;
  QPAYLOAD_FW_ENTRY   *pFwEntry     = NULL;
  FW_LINKED_LIST_TYPE *FwEntryList = NULL;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERROR_LABEL(Status,FwGetFwEntryList,StateFwEntryRollbackAllStartExit);

  Status = LinkedListGetHeadNode(FwEntryList,&Node);
  HANDLE_ERROR_LABEL(Status,LinkedListGetHeadNode,StateFwEntryRollbackAllStartExit);

  if (Node == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
    DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryRollbackAllStartExit);
  }

  Status= LinkedListGetNodeCount(FwEntryList,&NodeCount);
  HANDLE_ERROR_LABEL(Status,LinkedListGetNodeCount,StateFwEntryRollbackAllStartExit);

  for (i = 0; i < NodeCount; i++)
  {
    if (Node != NULL)
    {
      DEBUG((EFI_D_ERROR,"Rollingback %d/%d FwEntry..\n",i+1,NodeCount));
      pFwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;

      if (pFwEntry != NULL)
      {
        /* LUN1 and LUN2 will not be roll back */
        if ((pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN1)
         || (pFwEntry->Metadata.UpdatePath.DiskPartitionType == FWENTRY_UFS_LUN2))
        {
          DEBUG((EFI_D_ERROR, "  Rollback Success\n\n"));
          Node = Node->Next;
          continue;
        }
        
        Status = FwEntryRollback(pFwEntry);
        if(EFI_ERROR(Status))
        {
          FwPrintFwEntry(pFwEntry, EFI_D_ERROR);
          DEBUG((EFI_D_ERROR,"  Rollback failed\n\n"));
        }
        else
        {
          DEBUG((EFI_D_ERROR,"  Rollback Success\n\n"));
        }
        HANDLE_ERR_CONTINUE(Status,FwEntryRollback);
      }
      else
      {
        DEBUG((EFI_D_ERROR,"Node->Data == NULL\n"));
      }
      Node = Node->Next;
    }
    else
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG_ERR_LABEL("ListNode == NULL ",StateFwEntryRollbackAllStartExit);
    }
  }
  Status = UpdateStateinFS(FW_STATE_ROLLBACK_DONE);
  /*We want rollback to continue if UpdateStateinFS failed*/
  HANDLE_ERR_CONTINUE(Status,UpdateStateinFS);

  return EFI_SUCCESS;

StateFwEntryRollbackAllStartExit:

  FwPrintFwList(FwEntryList,EFI_D_ERROR);

  return Status;

}

/**
 *  Function to Rollback all FW entries
 *
 * @return EFI_STATUS
 */
EFI_STATUS
StateFwEntryPostRollbackStart(
  )
{
  EFI_STATUS          Status        = EFI_SUCCESS;
  FW_LINKED_LIST_TYPE *FwEntryList  = NULL;
  BOOLEAN             SwitchBootGpt = FALSE;
  BOOLEAN             SwitchGppGpt  = FALSE;
  UINTN               NodeCount     = 0;
  QPAYLOAD_FW_ENTRY   *pFwEntry     = NULL;
  FW_LIST_NODE_TYPE   *Node         = NULL;
  UINTN               i             = 0;

  Status = FwGetFwEntryList(&FwEntryList);
  HANDLE_ERR_CONTINUE(Status,FwGetFwEntryList);

  // Check if GPT tables has been switched
  Status = GetAllSwitchGptRequestsFromFwEntryList(FwEntryList, &SwitchBootGpt, &SwitchGppGpt);
  HANDLE_ERROR_LABEL(Status,GetAllSwitchGptRequestsFromFwEntryList,StateFwEntryPostRollbackStartExit);

  Status = RestoreAllGPTTablesFromFat(PRIMARY_GPT_TABLE, SwitchBootGpt, SwitchGppGpt);
  HANDLE_ERR_CONTINUE(Status,RestoreAllGPTTablesFromFat);

  Status = RestoreAllGPTTablesFromFat(BACKUP_GPT_TABLE, SwitchBootGpt, SwitchGppGpt);
  HANDLE_ERR_CONTINUE(Status,RestoreAllGPTTablesFromFat);

  for (i = 0; i < NodeCount; i++)
  {
    DEBUG((EFI_D_WARN,"  Deleting %d/%d FwEntry in rollback path...\n",i+1,NodeCount));

    if (Node != NULL)
    {
      pFwEntry = (QPAYLOAD_FW_ENTRY*)Node->Data;
      if (pFwEntry != NULL)
      {
        FwPrintFwEntry(pFwEntry, EFI_D_WARN);
        Status = FwEntryDeleteBackup(pFwEntry);
        if(EFI_ERROR(Status))
        {
          DEBUG((EFI_D_ERROR,"    Delete failed...Continue\n\n"));
        }
        else
        {
          DEBUG((EFI_D_ERROR,"    Delete Success\n\n"));
        }
        HANDLE_ERR_CONTINUE(Status,FwEntryDeleteBackup);
      }
      else
      {
        DEBUG((EFI_D_ERROR,"  Can not delete a NULL FwEntry\n\n"));

      }
      Node = Node->Next;
    }
    else
    {
      DEBUG((EFI_D_ERROR,"  Can not delete a NULL Node\n\n"));
      break;
    }
  }

  Status = UpdateStateinFS(FW_STATE_NOT_IN_PROGRESS);
  /*We want post rollback to continue if UpdateStateinFS failed*/
  HANDLE_ERR_CONTINUE(Status,UpdateStateinFS);

StateFwEntryPostRollbackStartExit:

  return Status;

}


/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[out] MsftAttemptStatus Result values corresponding to MSFT definitions.
  @param[out] QcResultFlag      Result values corresponding to QCOM definitions

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
FwUpdatePhaseMain (
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag

  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_STATUS PhaseStatus = EFI_SUCCESS;
  UINT8 AttemptCnt = 0;

  if((MsftAttemptStatus == NULL) ||
     (QcResultFlag == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag = QCFW_UPDATE_INIT;

  Status = UpdateHandleRegistry ();
  if (EFI_ERROR(Status))
  {
    *QcResultFlag = QCFW_UPDATE_ERR_HANDLE_REGISTRY;
    HANDLE_ERROR_LABEL(Status,UpdateHandleRegistry,FwUpdatePhaseMainExit);
  }

  while (FWupdateMode != QCOM_FW_UPDATE_MODE_NOUPDATE)
  {
    Status = GetState(&State) ;
    if (EFI_ERROR(Status))
    {
      *QcResultFlag = QCFW_UPDATE_ERR_FLAG_GET_STAMACHINE_STATE;
      HANDLE_ERROR_LABEL(Status,GetState,FwUpdatePhaseMainExit);
    }

    Status = GetAttemptCount(&AttemptCnt);
    if (EFI_ERROR(Status))
    {
      AttemptCnt = 0;
    }

    /* If update is in progress then need to perform a roll-back to abort if too many attempts have been made.*/
    if ((AttemptCnt > UPDATE_ATTEMPT_COUNT_MAX) &&
        (State != FW_STATE_ROLLBACK_DONE))
    {
      *QcResultFlag = QCFW_UPDATE_ERR_FLAG_STATE_EXCEEDED_UPDATE_ATTEMPT_MAX;
      if (State == FW_STATE_NOT_IN_PROGRESS)
      {
        /*Rollback is not required, */
        DEBUG_ERR_LABEL("\nAborting. Reached update attempt limit.\n",FwUpdatePhaseMainExit);
      }
      else
      {
        /*Needs to rollback all*/
        DEBUG((EFI_D_WARN,"\nAborting. Reached update attempt limit. Current update attempt count is %d\n\n", AttemptCnt));
        State = FW_STATE_UPDATE_FAIL;
        Status = UpdateStateinFS(FW_STATE_UPDATE_FAIL);
        HANDLE_ERROR_LABEL(Status,UpdateStateinFS,FwUpdatePhaseMainExit);
      }
    }

    switch (State)
    {
    case FW_STATE_NOT_IN_PROGRESS:
      DEBUG((EFI_D_WARN,"\n  Phase 1: FwEntry Backup start\n\n"));
      Status = StateFwEntryBackupStart(MsftAttemptStatus,QcResultFlag);
      HANDLE_ERROR_LABEL(Status,StateFwEntryBackupStart,FwUpdatePhaseMainExit);
      DEBUG((EFI_D_WARN,"\n  Phase 1 Done\n"));
      break;

    case FW_STATE_BACKUP_DONE:
      DEBUG((EFI_D_WARN,"\n  Phase 2: FwEntry Update start\n\n"));
      Status = StateFwEntryUpdateStart(MsftAttemptStatus,QcResultFlag);
      HANDLE_ERR_CONTINUE(Status,StateFwEntryUpdateStart);
      if (EFI_ERROR(Status))
      {
        HANDLE_ERROR_LABEL(UpdateStateinFS(FW_STATE_UPDATE_FAIL),UpdateStateinFS,FwUpdatePhaseMainExit);
      }
      DEBUG((EFI_D_WARN,"\n  Phase 2 Done\n"));
      break;

    case FW_STATE_UPDATE_SUCCESS:
      DEBUG((EFI_D_WARN,"\n  Phase 3: FwEntry Post Update start\n\n"));
      Status = StateFwEntryPostUpdateStart(MsftAttemptStatus,QcResultFlag);
      HANDLE_ERR_CONTINUE(Status,StateFwEntryUpdateStart);
      if (EFI_ERROR(Status))
      {
        HANDLE_ERROR_LABEL(UpdateStateinFS(FW_STATE_UPDATE_FAIL),UpdateStateinFS,FwUpdatePhaseMainExit);
      }
      DEBUG((EFI_D_WARN,"\n  Phase 3 Done\n"));
      goto FwUpdatePhaseMainExit;

    case FW_STATE_UPDATE_FAIL:
      DEBUG((EFI_D_WARN,"\n  Error Phase 1 : FwEntry rollback start\n\n"));
      Status = StateFwEntryRollbackAllStart();
      if (EFI_ERROR(Status))
      {
        HANDLE_ERROR_LABEL(UpdateStateinFS(FW_STATE_ROLLBACK_DONE),UpdateStateinFS,FwUpdatePhaseMainExit);
      }
      DEBUG((EFI_D_WARN,"\n  Error Phase1 Done\n"));
      break;

    case FW_STATE_ROLLBACK_DONE:
      DEBUG((EFI_D_WARN,"\n  Error Phase 2:  Post rollback start\n\n"));
      // On an abnormal reset just after recording FW_STATE_UPDATE_FAIL, the device will continue
      // with rollback stage. At this point, PhaseStatus will have EFI_SUCESS instead of the error code
      // from update phase. So if PhaseStatus is EFI_SUCCESS while entering the rollback state, 
      // update the error to a generic error code so that update status is reported as a failure.
      if (PhaseStatus == EFI_SUCCESS)
      {
          PhaseStatus = EFI_DEVICE_ERROR;
      }
      Status = StateFwEntryPostRollbackStart();
      HANDLE_ERROR_LABEL(Status,StateFwEntryPostRollbackStart,FwUpdatePhaseMainExit);
      DEBUG((EFI_D_WARN,"\n  Error Phase 2 Done\n"));
      goto FwUpdatePhaseMainExit;

    default:
      return EFI_LOAD_ERROR ;
    }
    // Preserve the first error encountered
    PhaseStatus = EFI_ERROR(PhaseStatus) ? PhaseStatus : Status;
  }

FwUpdatePhaseMainExit:

  return (EFI_ERROR(PhaseStatus) ? PhaseStatus : Status);
}

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
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_SYSTEM_RESOURCE_ENTRY  ent;

  *MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  *QcResultFlag      = QCFW_UPDATE_INIT;

  FWupdateMode       = QCOM_FW_UPDATE_MODE_NOUPDATE;

  //Get Block I/O and File Handles
  Status = UpdateHandleRegistry();
  HANDLE_ERROR_LABEL(Status,UpdateHandleRegistry,RunFwUpdateMainExit);

  Status = CheckandCreateFilesinFat() ;
  HANDLE_ERROR_LABEL(Status,CheckandCreateFilesinFat,RunFwUpdateMainExit);

  Status = QcomGetEsrtEntryCache(&ent);
  if (EFI_ERROR(Status))
  {
    HANDLE_ERROR_LABEL(Status,QcomGetEsrtEntryCache,RunFwUpdateMainExit);
  }

  QcomPrintEsrtEntry(&ent,EFI_D_WARN);

  FWupdateMode = QCOM_FW_UPDATE_MODE_CAPSULE;

  // TODO: Sumit: Needs to check if we have any problem related to PCR7 
  Status = FwUpdateCapsulePhaseMain((VOID *)Image,ImageSize,MsftAttemptStatus,QcResultFlag);
  HANDLE_ERR_CONTINUE(Status,FwUpdateCapsulePhaseMain);

RunFwUpdateMainExit:
 /*only SUCCESS here, shouldn't error out*/
  return Status;
}

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
  )
{
  EFI_STATUS Status               = EFI_SUCCESS;
  UINT32     Version              = 0;
  UINTN      PayloadHeaderV3Size  = sizeof (QPAYLOAD_METADATA_HEADER_V3);

  if ((Image == NULL) ||
      (PayloadHeaderV3 == NULL) ||
      (MsftAttemptStatus == NULL) ||
      (QcResultFlag == NULL))
  {
      Status = EFI_INVALID_PARAMETER;
      goto ExtractSystemFirmwareMetaDataExit;
  }
  
  /* Find payload format version from MetaData */
  Status = FwFindMetadata(
    Image,
    ImageSize,
    &Version,
    PayloadHeaderV3,
    &PayloadHeaderV3Size,
    NULL,
    NULL,
    MsftAttemptStatus,
    QcResultFlag
    );
  HANDLE_ERROR_LABEL(Status,FwFindMetadata,ExtractSystemFirmwareMetaDataExit);

ExtractSystemFirmwareMetaDataExit:

  return Status;
}
